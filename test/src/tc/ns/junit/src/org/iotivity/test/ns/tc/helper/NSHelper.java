//******************************************************************
//
// Copyright 2017 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

package org.iotivity.test.ns.tc.helper;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;
import org.iotivity.service.ns.provider.*;
import org.iotivity.service.ns.common.*;
import org.iotivity.service.ns.common.SyncInfo.SyncType;
import org.iotivity.service.ns.consumer.*;
import org.iotivity.service.ns.consumer.Provider.ProviderState;

import static org.iotivity.test.ns.tc.helper.NSTestUtilily.*;
import android.content.Context;
import android.util.Log;

public class NSHelper implements ProviderService.OnConsumerSubscribedListener,
        ProviderService.OnMessageSynchronizedListener,
        ConsumerService.OnProviderDiscoveredListener,
        Provider.OnMessageReceivedListener, Provider.OnProviderStateListener,
        Provider.OnSyncInfoReceivedListener {
    private static NSHelper sNSHelperInstance   = null;
    private Context         mContext           = null;
    private Consumer        mConsumer          = null;
    private Provider        mProvider          = null;
    private ProviderService mProviderService   = null;
    private ConsumerService mConsumerService   = null;
    private Message         mMessage           = null;
    private ProviderState   mProviderState     = null;
    private SyncInfo        mProviderSyncInfo  = null;
    private SyncInfo        mConsumerSyncInfo  = null;
    private boolean         mIsProviderStarted = false;
    private boolean         mIsConsumerStarted = false;

    private NSHelper(Context context) {
        mContext = context;
        configPlatform();
    }

    public static synchronized NSHelper getInstance(Context context) {
        if (sNSHelperInstance == null) {
            sNSHelperInstance = new NSHelper(context);
        }

        return sNSHelperInstance;
    }

    private void configPlatform() {
        PlatformConfig platformConfig = new PlatformConfig(mContext,
                ServiceType.IN_PROC, ModeType.CLIENT_SERVER, "0.0.0.0", 0,
                QualityOfService.LOW);

        Log.i(TAG, "Configuring platform.");

        OcPlatform.Configure(platformConfig);
        try {
            OcPlatform.stopPresence();
        } catch (Exception e) {
            Log.e(TAG, "Exception: stopping presence when configuration step: "
                    + e);
        }
        Log.i(TAG, "Configuration done Successfully");
    }

    public ProviderService getProviderService() {
        if (mProviderService == null) {
            mProviderService = ProviderService.getInstance();
        }
        return mProviderService;
    }

    public ConsumerService getConsumerService() {
        if (mConsumerService == null) {
            mConsumerService = ConsumerService.getInstance();
        }
        return mConsumerService;
    }

    public void setProviderStartStatus(boolean isStarted) {
        mIsProviderStarted = isStarted;
    }

    public void setConsumerStartStatus(boolean isStarted) {
        mIsConsumerStarted = isStarted;
    }

    public void stopServices() {

        if (mIsConsumerStarted && (mConsumerService != null)) {
            try {
                mConsumerService.stop();
                mIsConsumerStarted = false;
                Log.i(TAG, "Consumer Service Stopped Successfully");
            } catch (NSException e) {
                Log.e(TAG, e.getMessage());
            }

            waitInSecond(WAIT_MIN_TIME + WAIT_MIN_TIME);
        }
        mConsumerService = null;

        if (mIsProviderStarted && (mProviderService != null)) {
            try {
                mProviderService.stop();
                mIsProviderStarted = false;
                Log.i(TAG, "Provider Service Stopped Successfully");

            } catch (NSException e) {
                Log.e(TAG, "Failed to stop Provider Service");
                Log.e(TAG, e.getMessage());
            }

            waitInSecond(WAIT_MIN_TIME);
        }
        mProviderService = null;
    }

    public boolean stopProviderService() {
        if (mProviderService == null) {
            return false;
        }

        try {
            mProviderService.stop();
        } catch (NSException e) {
            Log.e(TAG, "Can't stop provider service");
            e.printStackTrace();
            return false;
        }

        mIsProviderStarted = false;
        mProviderService = null;
        return true;
    }

    public Consumer getConsumer(boolean subControllability,
            boolean isTopicRegister) {
        mProvider = null;
        mConsumer = null;

        stopServices();
        mProviderService = getProviderService();
        mConsumerService = getConsumerService();

        try {
            mProviderService.start(this, this, subControllability, USER_INFO,
                    IS_SECURED);
            mIsProviderStarted = true;
            Log.d(TAG, "provider service started");

            if (isTopicRegister)
                mProviderService.registerTopic(TOPIC_NAME);
            Log.d(TAG, "topic registered");

            waitInSecond(WAIT_MIN_TIME);

            mConsumerService.start(this);
            mIsConsumerStarted = true;
            Log.d(TAG, "consumer service started");

            waitInSecond(WAIT_MIN_TIME);

            mConsumerService.rescanProvider();
            waitInSecond(WAIT_MIN_TIME * 3);
        } catch (NSException e) {
            Log.e(TAG, "Can't Start Consumer Service");
            return null;
        }

        if (subControllability) {
            boolean isFound = waitForService(WAIT_MAX_TIME, false);
            Log.d(TAG, "waiting for provider with provider control");

            if (!isFound) {
                Log.w(TAG, "Can't find provider");
            }
        } else {
            boolean isFound = waitForService(WAIT_MAX_TIME, true);
            Log.d(TAG, "waiting for provider with consumer control");
            if (isFound) {
                try {
                    mProvider.subscribe();
                } catch (NSException e) {
                    Log.e(TAG, "Can't Subscribe to provider");
                }
            } else {
                Log.e(TAG, "Can't find provider");
            }

            isFound = waitForService(WAIT_MAX_TIME, false);

            if (isFound) {
                Log.i(TAG, "Consumer Found Successfully");
            } else {
                Log.w(TAG, "Can't find any Consumer");
            }
        }

        return mConsumer;
    }

    public Provider getProvider(boolean subControllability) {
        mProvider = null;
        mConsumer = null;

        mProviderService = ProviderService.getInstance();
        mConsumerService = ConsumerService.getInstance();

        try {
            mProviderService.start(this, this, subControllability, USER_INFO,
                    IS_SECURED);
            mProviderService.registerTopic(TOPIC_NAME);
            mIsProviderStarted = true;
            waitInSecond(WAIT_MIN_TIME);

            mConsumerService.start(this);
            mIsConsumerStarted = true;
            mConsumerService.rescanProvider();
        } catch (NSException e) {
            Log.e(TAG, "Can't Start. Exception is " + e.getLocalizedMessage());
        }

        waitForService(WAIT_MAX_TIME, true);

        return mProvider;
    }

    public boolean sendNotification() {
        if (mProviderService == null || mConsumer == null) {
            Log.e(TAG, "ProviderService or Consumer is null");
            return false;
        }

        try {
            mProviderService.registerTopic(TOPIC_NAME);
            mConsumer.setTopic(TOPIC_NAME);
        } catch (NSException e) {
            e.printStackTrace();
            return false;
        }

        Message msg = null;

        try {
            msg = mProviderService.createMessage();
            msg.setTitle(MSG_TITLE);
            msg.setContentText(MSG_BODY);
            msg.setSourceName(MSG_SOURCE);
            msg.setTopic(TOPIC_NAME);

            MediaContents mediaContents = new MediaContents("Image");

            msg.setMediaContents(mediaContents);
        } catch (NSException e) {
            e.printStackTrace();
            return false;
        }

        try {
            mMessage = null;
            mProviderService.sendMessage(msg);
        } catch (NSException e) {
            Log.e(TAG, "SendMessage is failed.");
            e.printStackTrace();
            return false;
        }

        Log.i(TAG, "Notification Sent");

        return true;
    }

    public Message getNotification() {
        return mMessage;
    }

    private boolean waitForService(int timeOut, boolean isProvider) {
        int progessTime = 0;

        while (true) {
            if (isProvider)
                Log.d(TAG, "Waiting for Provider " + progessTime + "...");
            else
                Log.d(TAG, "Waiting for Consumer " + progessTime + "...");

            if (isProvider) {
                if (mProvider != null) {
                    Log.i(TAG, "Provider found with ID: "
                            + mProvider.getProviderId());
                    return true;
                }
            } else {
                if (mConsumer != null) {
                    Log.i(TAG, "Consumer found with ID: "
                            + mConsumer.getConsumerId());
                    return true;
                }
            }

            waitInSecond(WAIT_MIN_TIME);

            if (++progessTime > timeOut) {
                break;
            }
        }

        return false;
    }

    public void waitInSecond(int time) {
        int timeInSec = time * 1000;
        try {
            Thread.sleep(timeInSec);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    public boolean sendSyncInfo(boolean isProvider, SyncType type) {
        if (mMessage == null) {
            Log.e(TAG, "Notification is null");
            return false;
        }

        if (isProvider) {
            if (mProviderService == null) {
                Log.e(TAG, "Provider service is null");
                return false;
            }
        } else {
            if (mProvider == null) {
                Log.e(TAG, "Provider is null");
                return false;
            }
        }

        try {
            if (isProvider)
                mProviderService.sendSyncInfo(mMessage.getMessageId(), type);
            else
                mProvider.sendSyncInfo(mMessage.getMessageId(), type);
        } catch (NSException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public SyncInfo getSyncInfo(boolean isProvider) {
        if (isProvider)
            return mProviderSyncInfo;
        else
            return mConsumerSyncInfo;
    }

    public ProviderState getProviderState() {
        return mProviderState;
    }

    // Provider Callback
    @Override
    public void onMessageSynchronized(SyncInfo syncInfo) {
        Log.i(TAG, "onMessageSynchronized Listener Called...");

        mConsumerSyncInfo = syncInfo;

        printSyncInfo(syncInfo);
    }

    @Override
    public void onConsumerSubscribed(Consumer consumer) {
        Log.i(TAG, "onConsumerSubscribed Listener Called with ConsumerID: "
                + consumer.getConsumerId() + "...");
        mConsumer = consumer;
    }

    // Consumer Callback
    @Override
    public void onSyncInfoReceived(SyncInfo sync) {
        Log.i(TAG, "onSyncInfoReceived Listener Called ...");

        mProviderSyncInfo = sync;

        printSyncInfo(sync);
    }

    @Override
    public void onProviderStateReceived(ProviderState state) {
        Log.i(TAG, "onProviderStateReceived Listener Called...");

        mProviderState = state;
    }

    @Override
    public void onMessageReceived(Message message) {
        Log.i(TAG, "onMessageReceived Listener Called...");

        mMessage = message;

        printMessage(mMessage);
    }

    @Override
    public void onProviderDiscovered(Provider provider) {
        Log.i(TAG, "onProviderDiscovered Listener Called with ProviderID: "
                + provider.getProviderId() + "...");

        mProvider = provider;

        try {
            mProvider.setListener(this, this, this);
        } catch (NSException e) {
            Log.e(TAG, "Can't set listener");
        }
    }

    void printMessage(Message message) {
        String log = "MessageID: " + message.getMessageId();
        log += " Title = " + message.getTitle();
        log += " Body = " + message.getContentText();
        log += " Topic = " + message.getTopic();
        log += " ProviderID = " + message.getProviderId();

        Log.i(TAG, log);
    }

    void printSyncInfo(SyncInfo info) {
        try {
            Log.i(TAG,
                    "SyncInfo --> ProviderID: " + info.getProviderId()
                            + " MessageID: " + info.getMessageId() + " State: "
                            + getSyncType(info.getState()));
        } catch (Exception e) {
            Log.w(TAG, "Can't print syncInfo. Exception: "
                    + e.getLocalizedMessage());
        }
    }

    String getSyncType(SyncType type) {
        switch (type) {
            case UNREAD:
                return "UNREAD";

            case READ:
                return "READ";

            case DELETED:
                return "DELETED";

            default:
                return "UNKNOWN";
        }
    }
}
