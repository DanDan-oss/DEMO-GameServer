#ifndef TIMER_H
#define TIMER_H
#include <zinx.h>
#include <vector>
#include <list>

class TimerChannel :public Ichannel
{
public:
	// 通过 Ichannel 继承
	virtual bool Init() override;
	virtual bool ReadFd(std::string& _input) override;
	virtual bool WriteFd(std::string& _output) override;
	virtual void Fini() override;
	virtual int GetFd() override;
	virtual std::string GetChannelInfo() override;
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
private:
	int m_timerFd = -1;
};

class TimerOutProc {
public:
	virtual void Proc() = 0;
	virtual int GetTimeSec() = 0;
	int iCount = -1;		/*所剩圈数*/
};

class TimerManage :public AZinxHandler
{
	// 通过 AZinxHandler 继承
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;		// 流程处理
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
public:
	TimerManage();
	void AddTask(TimerOutProc* _ptask);		// 添加定时器任务
	void DelTask(TimerOutProc* _ptask);		// 删除定时器任务(添加到删除队列,合适的时机删除)

private:
	void InitTimerManage(unsigned int _maxindex);	// 初始化时间轮链表
	void DispatchRun();		// 时间轮执行一个刻度调度链表
	void DelLinkRun();		// 将调度链表轮循一圈后仍未找到的定时器删除

	int timer_current_index = 0;
	int timer_max_index = 0;
	std::vector< std::list<TimerOutProc*> > timer_wheel;	// 时间轮 <调度链表>
	std::vector< std::list<TimerOutProc*> > timer_del_list;	// 时间轮 <删除链表>

public:
	static TimerManage* GetInstance() {
		return TimerManage::poSingle;
	}
private:
	static TimerManage* poSingle;		// 当前时钟管理 单例对象指针
};

#endif // !TIMER_H
