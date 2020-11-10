/*
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 * Copyright 2018 Intel Corporation All Rights Reserved.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#include <signal.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include <pthread.h>

#include "json/json.h"

// IPC PROP
#define PID_AGENT 1
#define PID_ETRI 2
#define PID_OCF 3

#define MSG_SIZE 1024
#define ETRI_QUEUE_KEY 1
#define OCF_QUEUE_KEY 2

#define MY_PID PID_ETRI

// IPC REQ_TYPE
#define GET_REQ "GET_REQ"
#define GET_RSP "GET_RSP"
#define POST_REQ "POST_REQ"
#define POST_RSP "POST_RSP"
#define CREATE_REQ "CREATE_REQ"
#define CREATE_RSP "CREATE_RSP"

// IPC JSON KEY
const std::string KEY_REQ_TYPE = "req_type";
const std::string KEY_RESOURCES = "resources";
const std::string KEY_RESULT = "result";
const std::string KEY_ID = "id";
const std::string KEY_URI = "uri";

typedef struct
{
	long mtype;
	char data[MSG_SIZE];
} msg_data;

bool init;
int etri_msgid; // queue_id
Json::FastWriter fw;
int que_id = msgget((key_t)ETRI_QUEUE_KEY, IPC_CREAT | 0666);

void sendMsg(Json::Value value, long dst_pid)
{
	msg_data send_msg;
	send_msg.mtype = dst_pid;
	std::string out = fw.write(value);
	strcpy(send_msg.data, out.c_str());
	int msg_size = sizeof(send_msg) - sizeof(send_msg.mtype);
	int rtn = msgsnd(que_id, &send_msg, msg_size, IPC_NOWAIT);
	std::cout << "rtn : " << rtn << " queue_id : " << que_id << std::endl;
}

Json::Value createResource(std::string uri, std::string id)
{
	Json::Value result;
	result["uri"] = uri;
	result["id"] = id;

	Json::Value props;
	Json::Value p1;

	p1["name"] = "dc";

	props.append(p1);

	result["p"] = props;
	return result;
}
void *create(void *p)
{
	// start ocf resource scan

	Json::Value resources;
	resources.append(createResource("/temperature", "1234"));
	resources.append(createResource("/humidity", "1234"));

	Json::Value value;
	value["req_type"] = "CREATE_REQ";
	value["resources"] = resources;

	while (true)
	{
		if (!init)
		{
			sendMsg(value, PID_AGENT);
		}
		sleep(5);
	}
}

void *recv_queue(void *arg)
{
	msg_data msg;
	int msg_size = sizeof(msg) - sizeof(msg.mtype);
	// etri message queue init
	etri_msgid = msgget((key_t)ETRI_QUEUE_KEY, IPC_CREAT | 0666);
	if (etri_msgid == -1)
	{
		std::cout << "etri_msgid msgget() Error" << std::endl;
	}
	else
	{
		//etri queue clear
		while (msgrcv(etri_msgid, &msg, msg_size, 0, IPC_NOWAIT) > 0)
		{
		}
	}

	std::cout << "etri_msgid = " << etri_msgid << std::endl;

	Json::Value value;
	Json::Reader reader;

	for (;;)
	{

		if (msgrcv(etri_msgid, &msg, msg_size, MY_PID, IPC_NOWAIT) > 0)
		{

			// char[] to String
			std::string data(msg.data);

			// parse String to JSON
			bool success = reader.parse(data, value);
			if (success)
			{

				// Check JSON has Key 'req_type'
				if (value.isMember(KEY_REQ_TYPE))
				{
					std::string req_type = value["req_type"].asString();

					// Check REQ_TYPE
					if (req_type.compare(CREATE_RSP) == 0)
					{

						std::cout << "## CREATE_RSP" << std::endl;
						if (value.isMember(KEY_RESULT))
						{
							// createThread Stop
							init = true;
						}
						// if not added , create ContentResource
					}
					else if (req_type.compare(GET_REQ) == 0)
					{
						std::cout << "## GET_REQ" << std::endl;
						Json::Value rsp;
						rsp["req_type"] = GET_RSP;

						if (value["uri"].asString().compare("/temperature") == 0)
						{
							Json::Value data;
							data["temperature"] = 24.5;
							data["dc"] = true;

							rsp["/temperature"] = data;
						}
						else if (value["uri"].asString().compare("/humidity") == 0)
						{
							Json::Value data;
							data["humidity"] = 55;
							data["desiredHumidity"] = 55;
							data["dc"] = true;

							rsp["/humidity"] = data;
						}

						std::cout << rsp << std::endl;
						sendMsg(rsp, PID_AGENT);
					}
					else if (req_type.compare(POST_REQ) == 0)
					{
						std::cout << "## POST_REQ" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "json parse Error" << data << std::endl;
			}

			std::cout << "data : " << data << std::endl;
		}
	}
}

void startCreate()
{
	pthread_t id;
	pthread_create(&id, NULL, create, (void *)0);
}

void startRecv()
{
	pthread_t id;
	pthread_create(&id, NULL, recv_queue, (void *)0);
}

int main(int argc, char **argv)
{
	startRecv();
	startCreate();
	while (true)
	{
	}
	return 0;
}
