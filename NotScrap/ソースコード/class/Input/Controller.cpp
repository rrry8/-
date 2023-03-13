#include<DxLib.h>
#include "Controller.h"
#include "../../_debug/_DebugDispOut.h"


Controller::RingBuf* Controller::RingBuf::Create(int no)//返り値の場合は名前空間を指定してあげる
{
	int count = 0;
	auto beginBuf = new RingBuf{count,nullptr,nullptr};
	auto beforBuf = beginBuf;
	auto nowBuf = beginBuf;
	for (int j = 1; j < no; j++)
	{
		count++;
		nowBuf = new RingBuf{ count,beforBuf,nullptr };
		beforBuf->next = nowBuf;
		beforBuf = nowBuf;
	}
	beginBuf->befor = nowBuf;
	nowBuf->next = beginBuf;

	return beginBuf;

}

void Controller::RingBuf::Destroy(RingBuf* buf)//引数の場合はクラスからとれる
{
	if (buf->befor == nullptr)
	{
		return;
	}
	buf->befor->next = nullptr;	//一個前から現在へのポインタを断ち切る
	while (buf->next != nullptr)	//次がnullじゃないなら現在をdelete。次がnullなら抜ける
	{
		buf = buf->next;
		delete buf->befor;
	}
	delete buf;//現在をdelete
}

void Controller::WriteCmd(void)
{
	/*int writeData = 0;
	if (writeData)
	{

		if(ringBuf_->value() != writeData)
	}*/

	bool flag = false;
	for (auto id : InputID())
	{
		if (cntData_[id][static_cast<int>(Trg::Now)])
		{
			ringBuf_ = ringBuf_->next;
			ringBuf_->value = static_cast<int>(id);
			flag = true;
		}
	}
	if (!flag)
	{
		startBuf_ = ringBuf_;
	}
	else
	{
		if (startBuf_ == ringBuf_)//スタート位置をずらす
		{
			startBuf_ = startBuf_->next;
		}
	}

	auto drawBuf = startBuf_;
	int no = 0;
	while (drawBuf != ringBuf_)
	{
		_dbgDrawFormatString(no * 32, 560, 0xffffff, "%d", drawBuf->value);
		drawBuf = drawBuf->next;
		no++;
	}
}
