#ifndef VIDEOCAPREINPUT_H
#define VIDEOCAPREINPUT_H

#include <string>
#include <unordered_map>
#include <functional>
#include <thread>


namespace rz {
	class VideoCapInput
	{
	public:
		VideoCapInput();
		~VideoCapInput();
		
		bool start();
		bool stop();
		void setDeviceName(std::string dev)
		{
			//m_strDeviceid = dev;
			if (vruning)
			{
				stop();
				start();
			}
		}

		void SetResolution(int w, int h)
		{
			m_nWidth = w;
			m_nHeight = h;
			if (vruning)
			{
				stop();
				start();
			}
		}

		std::string GetCurDeviceName()
		{
			//return m_strDeviceid;
		}

		bool GetState()
		{
			return vruning;
		}
	public:
		std::function<void(uint8_t*, int, int)> iyuvoutfun = nullptr;
	private:
		void threadFunc();
		void setvideoformat(std::string s) {
			videoformat = s;
		}
		void printErrInfo(const std::string& info, int moduleCode, int paasCode);
	private:
		std::string videoformat;
		volatile bool vruning = false;
		std::thread m_t;
		int m_nFrameRate = 15;
		int m_nWidth = 1280;
		int m_nHeight = 720;
		
	};

	class VideoCapInputMngr
	{
	public:
		static VideoCapInputMngr*instance();
		VideoCapInput* GetVideoCapInput();

	private:
		static VideoCapInputMngr* m_pVideoCapInputMngr;
		VideoCapInput* m_pVideoCapInput = nullptr;
	};
}

#endif //VIDEOCAPTUREFFMPEG_H
