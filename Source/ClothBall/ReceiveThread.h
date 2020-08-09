// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ThreadingBase.h"
#include "Networking.h"
#include <string>
#include <vector>

class CLOTHBALL_API FReceiveThread : public FRunnable
{
public:
	FReceiveThread(FSocket* client) : m_Client(client)
	{}
	~FReceiveThread()
	{}

	virtual bool Init() override
	{
		stopping = false;
		return true;
	}
	TArray<uint8> ReceiveData;
	std::vector<int> s_int;
	bool data_received = false;
	bool IsNewDataComing = false;

	virtual uint32 Run() override
	{
		if (!m_Client)
		{
			return 0;
		}


		uint8 element = 0;
		//接收数据包
		data_received = false;
		
		while (!stopping)   //线程计数器控制
		{

			ReceiveData.Init(element, 1024u);
			int32 read = 0;
			m_Client->Recv(ReceiveData.GetData(), ReceiveData.Num(), read);
			const FString ReceivedUE4String = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceiveData.GetData())));
			FString log = "Server:" + ReceivedUE4String;
			//UE_LOG(LogTemp, Warning, TEXT("*** %s"), *log);



			//parse received data

			std::string reveivedString = std::string(TCHAR_TO_UTF8(*ReceivedUE4String));
			std::vector<std::string> s_str;
			s_str = split(reveivedString, " ");
			s_int.clear();
			for (int i = 0; i < s_str.size(); i++)
			{
				s_int.push_back(std::stoi(s_str[i]));
				//UE_LOG(LogTemp, Warning, TEXT("*** %d"), s_int[i]);
			}
			if (s_int.size() >= 4)
				data_received = true;
			IsNewDataComing = true;
			FPlatformProcess::Sleep(0.01f);


		}


		return 1;
	}   

	 virtual void Stop() override
	{

		 stopping = true;    //计数器-1
	}

	std::vector<std::string> split(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> res;
		if ("" == str) return res;
		//先将要切割的字符串从string类型转换为char*类型  
		char * strs = new char[str.length() + 1]; //不要忘了  
		strcpy(strs, str.c_str());

		char * d = new char[delim.length() + 1];
		strcpy(d, delim.c_str());

		char *p = strtok(strs, d);
		while (p) {
			std::string s = p; //分割得到的字符串转换为string类型  
			res.push_back(s); //存入结果数组  
			p = strtok(NULL, d);
		}

		return res;
	}

private:
	FSocket* m_Client;  //客户端套接字
	bool stopping;      //循环控制
	FThreadSafeCounter m_StopTaskCounter;   //线程引用计数器
};