#include "ZinxTimer.h"
#include <sys/timerfd.h>


TimerManage* TimerManage::poSingle = new TimerManage();	  // 定时器管理类

bool TimerChannel::Init()
{
	/* 初始化时钟描述符*/
	int timerfd = -1;
	struct itimerspec timer = { {3,0},{1,0} };

	timerfd = timerfd_create(CLOCK_REALTIME, 0);
	if (0 > timerfd)
		return false;
	if (timerfd_settime(timerfd, 0, &timer, nullptr))
		return false;
	this->m_timerFd = timerfd;
	return true;
}

bool TimerChannel::ReadFd(std::string& _input)
{
	char buf[8] = { 0 };
	if (sizeof(buf) != read(this->m_timerFd, buf, sizeof(buf)))
		return false;
	_input.assign(buf, sizeof(buf));
	return true;
}

bool TimerChannel::WriteFd(std::string& _output)
{
	return false;
}

void TimerChannel::Fini()
{
	if (0 <= this->m_timerFd)
	{
		close(this->m_timerFd);
		this->m_timerFd = -1;
	}
}

int TimerChannel::GetFd()
{
	return this->m_timerFd;
}

std::string TimerChannel::GetChannelInfo()
{
	return "TimerFd";
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return TimerManage::GetInstance();;
}

//==========================

TimerManage::TimerManage()
{
	this->InitTimerManage(10);
	return;
}


void TimerManage::InitTimerManage(unsigned int _index)
{
	this->timer_max_index = _index;
	std::list<TimerOutProc*> temp;
	for (int i = 0; i < (int)_index; ++i)
		this->timer_wheel.push_back(temp);
}

void TimerManage::AddTask(TimerOutProc* _ptask)
{
	int index = 0;
	int timeSec = _ptask->GetTimeSec();

	// 计算定时器在时间轮上的刻度
	index = (timeSec + this->timer_current_index) % this->timer_max_index;
	// 计算定时器在时间轮上的轮转圈数
	_ptask->iCount = timeSec / this->timer_max_index;
	// 添加到时间轮
	this->timer_wheel[index].push_back(_ptask);
	return;
}

void TimerManage::DelTask(TimerOutProc* _ptask)
{
	// 获取当前时间轮刻度,如果轮循一圈后未找到,将定时器对象从删除链表中删除
	//_ptask->iCount = this->timer_current_index;

	// 添加到删除链表
	//this->timer_del_list[timer_current_index].push_back(_ptask);


	for (auto& list : this->timer_wheel)
		for (auto temp : list)
			if (_ptask == temp)
				list.remove(temp);

	return;
}

void TimerManage::DispatchRun()
{
	this->timer_current_index = (this->timer_current_index+1) % this->timer_max_index;

	for (TimerOutProc* task : this->timer_wheel[this->timer_current_index])
	{
		task->iCount--;	// list索引--遍历
		if (task->iCount <= 0)
		{
			task->Proc();
			task->iCount = task->GetTimeSec();
		}

	}
}

void TimerManage::DelLinkRun()
{
	//TimerOutProc* temp = nullptr;
	//for (auto index : timer_del_list)
	//{
	//}
}

IZinxMsg* TimerManage::InternelHandle(IZinxMsg& _oInput)
{
	long long int lCount = 0;
	GET_REF2DATA(BytesMsg, msgCount, _oInput);
	msgCount.szData.copy((char*)&lCount, sizeof(long long int), 0);

	do
	{
		this->timer_current_index = (this->timer_current_index + 1) % this->timer_max_index;
		// 时间轮刻度里面的链表
		std::list<TimerOutProc*>& list = this->timer_wheel[this->timer_current_index];	
		std::list<TimerOutProc*> cache;
		for (auto itr = list.begin(); itr != list.end();)
		{
			if ((*itr)->iCount <= 0)
			{
				//auto temp = *itr;
				cache.push_back(*itr);
				AddTask(*itr);
				itr = list.erase(itr);
				continue;
			}
			(*itr)->iCount--;
			++itr;
		}
		for (auto temp : cache)
			temp->Proc();
	} while (lCount--);



	return nullptr;
}

AZinxHandler* TimerManage::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}