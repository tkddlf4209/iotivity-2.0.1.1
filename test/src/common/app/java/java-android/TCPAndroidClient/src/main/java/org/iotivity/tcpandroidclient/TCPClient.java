/******************************************************************
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************/

package org.iotivity.tcpandroidclient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import org.iotivity.configuration.ReadConfigPropFile;
import org.iotivity.configuration.IConfiguration;

public class TCPClient implements IConfiguration {
    private final String      TAG              = "TcpClient";
    private final String      TEMP_DIR         = "/data/local/tmp/";
    private String            serverMessage;
    private OnMessageReceived mMessageListener = null;
    private boolean           mRun             = false;

    private PrintWriter       mOut             = null;
    private BufferedReader    mIn              = null;
    private Context           mContext;

    /**
     * Constructor of the class. OnMessagedReceived listens for the messages
     * received from server
     */
    public TCPClient(final OnMessageReceived listener, Context context) {
        ReadConfigPropFile.readConfigFile(TEMP_DIR);
        mMessageListener = listener;
        mContext = context;
    }

    /**
     * Sends the message entered by client to the server
     *
     * @param message
     *            text entered by client
     */
    public void sendMessage(String message) {
        if (mOut != null && !mOut.checkError()) {
            System.out.println("message: " + message);
            mOut.println(message);
            mOut.flush();
        }
    }

    public void stopClient() {
        mRun = false;
    }

    public void run() {

        mRun = true;

        try {
            InetAddress serverAddr = InetAddress
                    .getByName(ReadConfigPropFile.s_mTcpCIServerIp);
            Socket socket = new Socket(serverAddr, SERVERPORT);
            try {

                mOut = new PrintWriter(new BufferedWriter(
                        new OutputStreamWriter(socket.getOutputStream())),
                        true);
                mIn = new BufferedReader(
                        new InputStreamReader(socket.getInputStream()));

                while (mRun) {
                    serverMessage = mIn.readLine();

                    if (serverMessage != null && mMessageListener != null) {
                        mMessageListener.messageReceived(serverMessage);
                        if (serverMessage
                                .equals("Server Initilization Complete")) {
                            Log.d(TAG,
                                    "BroadCasting message : " + serverMessage);
                            Intent startServer = new Intent("tcpClient");
                            startServer.putExtra("tcpClient", serverMessage);
                            mContext.sendBroadcast(startServer);
                        }
                    }
                    serverMessage = null;
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                socket.close();
            }

        } catch (Exception e) {
            Log.e("TCP SI Error", "SI: Error", e);
        }

    }

    public interface OnMessageReceived {
        public void messageReceived(String message);
    }
}