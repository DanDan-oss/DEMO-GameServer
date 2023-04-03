#ifndef TIMER_H
#define TIMER_H
#include <zinx.h>
#include <vector>
#include <list>

class TimerChannel :public Ichannel
{
public:
	// ͨ�� Ichannel �̳�
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
	int iCount = -1;		/*��ʣȦ��*/
};

class TimerManage :public AZinxHandler
{
	// ͨ�� AZinxHandler �̳�
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;		// ���̴���
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
public:
	TimerManage();
	void AddTask(TimerOutProc* _ptask);		// ��Ӷ�ʱ������
	void DelTask(TimerOutProc* _ptask);		// ɾ����ʱ������(��ӵ�ɾ������,���ʵ�ʱ��ɾ��)

private:
	void InitTimerManage(unsigned int _maxindex);	// ��ʼ��ʱ��������
	void DispatchRun();		// ʱ����ִ��һ���̶ȵ�������
	void DelLinkRun();		// ������������ѭһȦ����δ�ҵ��Ķ�ʱ��ɾ��

	int timer_current_index = 0;
	int timer_max_index = 0;
	std::vector< std::list<TimerOutProc*> > timer_wheel;	// ʱ���� <��������>
	std::vector< std::list<TimerOutProc*> > timer_del_list;	// ʱ���� <ɾ������>

public:
	static TimerManage* GetInstance() {
		return TimerManage::poSingle;
	}
private:
	static TimerManage* poSingle;		// ��ǰʱ�ӹ��� ��������ָ��
};

#endif // !TIMER_H
