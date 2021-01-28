#include "VideoCapInput.h"
#include <videoInput.h>
#include <libyuv.h>
#include <QDebug>
#include <OpenGLSignalMngr.cpp>
#include "RTC.h"
//#include "RzObject.h"

namespace rz {
	VideoCapInput::VideoCapInput()
	{

	}

	VideoCapInput::~VideoCapInput()
	{

	}

	bool VideoCapInput::start()
	{
		if (vruning)
			return true;

		QVariant value;
		

		if (m_t.joinable())
			m_t.join();
		m_t = std::thread(&VideoCapInput::threadFunc, this);
		vruning = true;

		return true;
	}

	bool VideoCapInput::stop()
	{
		vruning = false;
		if (m_t.joinable())
			m_t.join();
		return true;
	}

	void VideoCapInput::threadFunc()
	{
		videoInput VI;
		std::vector<std::string> devVec = videoInput::getDeviceList();
		
		if (devVec.size() <= 0)
		{
			return;
		}

		int index = 0;
		int tempw = m_nWidth;
		int temph = m_nHeight;
		
		VI.setupDevice(index, tempw, temph);
		if (!VI.isDeviceSetup(index))
		{
			printErrInfo("setupDevice failed", -1, 1504);
			return;
		}

		int width = VI.getWidth(index);
		int height = VI.getHeight(index);
		int imgSize = VI.getSize(index);
		unsigned char* dataBuffer = new unsigned char[imgSize];
		uint8_t* yuvbuffer = new uint8_t[width * height * 3 / 2];
		int count = 0;
        uint8_t* out = nullptr;
        int size;
        bool iskey;
		while (vruning)
		{
			if (count >= 1000)
			{
				printErrInfo("no video capture device detected", -1, 1501);
				vruning = false;
				break;
			}

			if (VI.isFrameNew(index) == false)
			{
				Sleep(50);
				count++;
				continue;
			}

			count = 0;

			if (VI.getPixels(index, dataBuffer, true, true) == false)
			{
				Sleep(5);
				continue;
			}

			libyuv::RGB24ToI420(dataBuffer, width * 3, yuvbuffer, width, yuvbuffer + height * width * 5 / 4, width / 2, yuvbuffer + height * width, width / 2, width, height);
		
			
			{
				//TODO 
				QByteArray arr((char*)yuvbuffer, width * height * 3 / 2);
				bool ret1 = QMetaObject::invokeMethod(OpenGLSignalMngr::instance(), "SigShowYuv", Qt::DirectConnection, Q_ARG(const QByteArray&, arr), Q_ARG(uint, width), Q_ARG(uint, height));
			}
			
			if (iyuvoutfun)
			{
                if (RTC::instance()->getEncoder()->encode(yuvbuffer, &out, size, iskey))
                    iyuvoutfun(out, size, iskey);
			}
			
			

			Sleep(30);
		}

		if (yuvbuffer)
			delete[] yuvbuffer;
		if (dataBuffer)
			delete[] dataBuffer;

		if (vruning)
			vruning = false;

		VI.stopDevice(index);
	}

	void VideoCapInput::printErrInfo(const std::string& info, int moduleCode, int paasCode)
	{
		std::string str = info;
		str += ", code is ";
		str += std::to_string(moduleCode);
		/*if (errorfunc) errorfunc(paasCode, str);*/
		qDebug() << QString::fromStdString(str);
	}

	VideoCapInputMngr* VideoCapInputMngr::m_pVideoCapInputMngr = nullptr;

	VideoCapInputMngr* VideoCapInputMngr::instance()
	{
		if (!m_pVideoCapInputMngr)
		{
			m_pVideoCapInputMngr = new VideoCapInputMngr;
		}
		return m_pVideoCapInputMngr;
	}

	VideoCapInput* VideoCapInputMngr::GetVideoCapInput()
	{
		if (!m_pVideoCapInput)
		{
			m_pVideoCapInput = new VideoCapInput;
		}
		return m_pVideoCapInput;
	}
}
