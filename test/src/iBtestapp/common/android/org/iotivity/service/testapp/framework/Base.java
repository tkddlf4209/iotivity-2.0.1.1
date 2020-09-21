/******************************************************************
*
* Copyright 2016 Samsung Electronics All Rights Reserved.
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
*
******************************************************************/

package org.iotivity.service.testapp.framework;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.ArrayList;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.text.method.ScrollingMovementMethod;
import android.view.Display;
import android.util.DisplayMetrics;
import android.os.StrictMode;
import android.os.Message;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class Base extends Activity {
    private static final String        REGISTER_DIALOG_MESSAGE        = "Register dialog first";
    private static final String        INPUT_TEXT_DIALOG_MESSAGE      = "Input text got in dialog : ";
    private static final String        SHOW_DIALOG_VALIDATION_MESSAGE = "Show Dialog First";
    private static final String        MENU_SEPERATOR_TEXT            = ". ";
    private static final String        SPLIT_TEXT                     = "\\ ";
    private static final String        NEW_LINE                       = "\n";
    private static final String        CANCEL                         = "CANCEL";
    private static final String        OK                             = "OK";
    private static final String        LOG_HEADER                     = "Log:\n";
    private static final String        INVOCATION_TARGET_EXCEPTION    = "InvocationTargetException";
    private static final String        ILLEGAL_ARGUMENT_EXCEPTION     = "IllegalArgumentException";
    private static final String        ILLEGAL_ACCESS_EXCEPTION       = "IllegalAccessException";
    private static Context             context;
    private static final String        EMPTY_TEXT_STRING              = "";
    private static final String        ACTION_NOT_FOUND_MESSAGE       = "Corresponding action not found";
    private static final String        ACTIONS_REGISTER_MESSAGE       = "Actions are not registered";
    private static final String        INVALID_MENU_OPTION_MESSAGE    = "Menu option does not exist";
    private static final String        ACTION_BUTTON_TEXT             = "Execute";
    private static final String        INPUT_HINT_TEXT                = "Input menu option number";
    private static String              outPutString                   = LOG_HEADER;
    private Object                     actionObject;
    private static TextView            outputTextView;
    private static AlertDialog         alertDialog;
    private static String              dialogInputText;
    private int                        screenWidth;
    private int                        screenHeight;
    Object                             syncObj                        = new Object();
    private static AlertDialog.Builder alert;
    private static boolean             isfinished;
    static Handler                     handler;
    private static boolean             mResult;
    public static ArrayList<String> mListInput = new ArrayList<>();

    private EditText inputEditTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        context = getApplicationContext();
        WindowManager wm = (WindowManager) context
                .getSystemService(Context.WINDOW_SERVICE);
        Display display = wm.getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);
        screenWidth = metrics.widthPixels;
        screenHeight = metrics.heightPixels;

        IntentFilter intentFilter = new IntentFilter("org.iotivity.test.INPUT_INTENT");
        registerReceiver(new InputReceiver(), intentFilter);
    }

    public static Context getContext() {
        return context;
    }

    public void RegisterApp(String appTitle,
            final Map<String, MenuInfo> menuMap, Object actionClassObj) {
        createMenu(appTitle, menuMap);
        registerActions(actionClassObj);
    }

    public void RegisterApp(String appTitle,
            final Map<String, MenuInfo> menuMap, Object actionClassObj,
            DialogInfo dialogInfo) {
        createMenu(appTitle, menuMap);
        registerActions(actionClassObj);
        if (dialogInfo != null) {
            createDialog(dialogInfo.title, dialogInfo.message);
        }
    }

    private void createMenu(String appTitle,
            final Map<String, MenuInfo> menuMap) {

        TableLayout.LayoutParams tableParams = new TableLayout.LayoutParams(
                TableLayout.LayoutParams.WRAP_CONTENT,
                TableLayout.LayoutParams.WRAP_CONTENT);
        TableRow.LayoutParams rowParams = new TableRow.LayoutParams(
                TableRow.LayoutParams.WRAP_CONTENT,
                TableRow.LayoutParams.WRAP_CONTENT);

        TableLayout parentLayout = new TableLayout(this);
        parentLayout.setLayoutParams(tableParams);

        TableRow titleBarRow = new TableRow(this);
        titleBarRow.setLayoutParams(tableParams);

        TableRow menuRow = new TableRow(this);
        menuRow.setLayoutParams(tableParams);

        TableRow inputRow = new TableRow(this);
        inputRow.setLayoutParams(rowParams);

        TableRow outputRow = new TableRow(this);
        outputRow.setLayoutParams(rowParams);

        GradientDrawable gd = new GradientDrawable();
        gd.setColor(Color.GRAY);
        gd.setCornerRadius(5);
        gd.setStroke(1, 0xFF000000);

        TextView titleBar = new TextView(this);
        titleBar.setText(appTitle);
        titleBar.setTextSize(20);
        titleBar.setGravity(Gravity.CENTER_VERTICAL);
        titleBar.setHeight(200);
        titleBar.setTypeface(Typeface.SERIF);
        titleBar.setTextColor(Color.BLACK);
        titleBarRow.setBackgroundDrawable(gd);
        titleBarRow.addView(titleBar);

        TextView menuTextView = new TextView(this);
        menuTextView.setLayoutParams(rowParams);
        menuTextView.setText(getMenuString(menuMap));
        menuTextView.setMaxWidth(menuTextView.getWidth() + 500);
        menuRow.addView(menuTextView);
        menuTextView.setTextSize(18);
        menuTextView.setHeight(screenHeight / 2);
        menuTextView.setVerticalScrollBarEnabled(true);
        menuTextView.setMovementMethod(new ScrollingMovementMethod());

        inputEditTextView = new EditText(this);

        menuTextView.setLayoutParams(rowParams);
        inputRow.addView(inputEditTextView);
        inputEditTextView.setHint(INPUT_HINT_TEXT);
        inputEditTextView.setSingleLine();

        final Button actionButton = new Button(this);
        actionButton.setLayoutParams(rowParams);
        actionButton.setText(ACTION_BUTTON_TEXT);
        inputRow.addView(actionButton);

        outputTextView = new TextView(this);
        outputTextView.setLayoutParams(rowParams);
        outputTextView.setVerticalScrollBarEnabled(true);
        outputTextView.setMovementMethod(new ScrollingMovementMethod());
        outputTextView.setMaxWidth(outputTextView.getWidth() + 500);
        outputRow.addView(outputTextView);

        parentLayout.addView(titleBarRow,
                new TableLayout.LayoutParams(
                        TableLayout.LayoutParams.MATCH_PARENT,
                        TableLayout.LayoutParams.WRAP_CONTENT));
        parentLayout.addView(menuRow,
                new TableLayout.LayoutParams(
                        TableLayout.LayoutParams.MATCH_PARENT,
                        TableLayout.LayoutParams.WRAP_CONTENT));
        parentLayout.addView(inputRow,
                new TableLayout.LayoutParams(
                        TableLayout.LayoutParams.MATCH_PARENT,
                        TableLayout.LayoutParams.WRAP_CONTENT));
        parentLayout.addView(outputRow,
                new TableLayout.LayoutParams(
                        TableLayout.LayoutParams.MATCH_PARENT,
                        TableLayout.LayoutParams.WRAP_CONTENT));
        setContentView(parentLayout);
        inputEditTextView.addTextChangedListener(new TextWatcher() {
            public void afterTextChanged(Editable s) {
            }

            public void beforeTextChanged(CharSequence s, int start, int count,
                    int after) {
            }

            public void onTextChanged(CharSequence s, int start, int before,
                    int count) {
            }
        });

        final InputHandler inputHandler = new InputHandler(menuMap);
        Thread thread = new Thread(inputHandler);
        thread.start();

        actionButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View arg0) {
                String inputText = inputEditTextView.getText().toString();
                synchronized(this){
                    mListInput.add(inputText);
                }
                InputMethodManager inputManager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                inputManager.hideSoftInputFromWindow(inputEditTextView.getWindowToken(), 0);
            }
        });
    }

    private String getMenuString(Map<String, MenuInfo> menuMap) {
        String text = EMPTY_TEXT_STRING;
        Set<Entry<String, MenuInfo>> set = menuMap.entrySet();
        Iterator<Entry<String, MenuInfo>> i = set.iterator();
        while (i.hasNext()) {
            Map.Entry<String, MenuInfo> me = (Map.Entry<String, MenuInfo>) i
                    .next();
            text += me.getKey() + MENU_SEPERATOR_TEXT;
            MenuInfo menuInfo = (MenuInfo) me.getValue();
            text += menuInfo.menuText + NEW_LINE;
        }
        return text;
    }

    protected static MenuInfo getMenuInfo(Map<String, MenuInfo> menuMap, String key) {
        MenuInfo menuInfo = menuMap.get(key);
        return menuInfo;

    }

    public static void showOutPut(final String outputText) {

        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Log.d("OutputTextBox", outputText);
                Base.outPutString += outputText + NEW_LINE;
                outputTextView.setText(outPutString);
            }
        };

        final Handler UIHandler = new Handler(Looper.getMainLooper());
        UIHandler.post(runnable);
    }

    public static void clearOutPut() {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                outPutString = LOG_HEADER;
                outputTextView.setText(outPutString);
            }
        };

        final Handler UIHandler = new Handler(Looper.getMainLooper());
        UIHandler.post(runnable);
    }

    private void registerActions(Object actionClassObj) {
        this.actionObject = actionClassObj;
    }

    private boolean isRegistered() {
        if (actionObject == null) {
            return false;
        }
        return true;
    }

    private Method getDeclaredMethod(Object obj, String name,
            Class<?>... parameterTypes) {
        if (isRegistered()) {
            try {
                return obj.getClass().getDeclaredMethod(name, parameterTypes);
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            } catch (SecurityException e) {
                e.printStackTrace();
            }
        } else
            showOutPut(ACTIONS_REGISTER_MESSAGE);
        return null;
    }

    

    private void createDialog(String title, String message) {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle(title);
        alert.setMessage(message);
        final EditText input = new EditText(this);
        alert.setView(input);
        alert.setPositiveButton(OK, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                dialogInputText = input.getEditableText().toString();
                mResult = true;
                handler.sendMessage(handler.obtainMessage());
            }
        });
        alert.setNegativeButton(CANCEL, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                mResult = false;
                handler.sendMessage(handler.obtainMessage());
                dialog.cancel();
            }
        });
        alertDialog = alert.create();
    }

    public static boolean showDialog() {
        handler = new Handler() {
            @Override
            public void handleMessage(Message mesg) {
                throw new RuntimeException();
            }
        };
        alertDialog.show();
        try {
            Looper.loop();
        } catch (RuntimeException e2) {
        }
        return mResult;
    }

    public static String getDialogText() {
        return dialogInputText;
    }

    private class InputHandler implements Runnable
    {
        private Map<String, MenuInfo> menuMap;

        InputHandler(Map<String, MenuInfo> menuMap)
        {
            this.menuMap = menuMap;
        }

        public void run()
        {
            while(true)
            {
                try
                {
                    Thread.sleep(1000);
                }
                catch(Exception ex)
                {
                }
                for(;;)
                {
                    String input = null;
                    synchronized (this)
                    {
                        if(mListInput.isEmpty())
                        {
                            break;
                        }
                        input = mListInput.get(0);
                        mListInput.remove(0);
                    }
                    executeAction(input);
                }
            }
        }

        private void executeAction(String inputText)
        {
            String key = EMPTY_TEXT_STRING;
            String argument = null;

            if (!inputText.isEmpty()) {
                if (inputText.contains(EMPTY_TEXT_STRING)) {
                    String temptext[] = inputText.split(SPLIT_TEXT);
                    key = temptext[0];
                    if (temptext.length > 1)
                        argument = temptext[1];
                } else {
                    key = inputText;
                }
                if (!menuMap.containsKey(key)) {
                    showOutPut(INVALID_MENU_OPTION_MESSAGE);
                    return;
                }
            } else {
                return;
            }

            MenuInfo menuInfo = getMenuInfo(menuMap, key);
            Method action = null;

            if (argument == null) {
                action = getDeclaredMethod(actionObject, menuInfo.actionName);
            } else {
                Class[] cArg = new Class[1];
                cArg[0] = String.class;
                action = getDeclaredMethod(actionObject, menuInfo.actionName,
                        cArg[0]);
            }
            if (null != action) {
                try {
                    if (argument == null) {
                        action.invoke(actionObject);
                    } else
                        action.invoke(actionObject, new String(argument));
                } catch (IllegalAccessException e) {
                    showOutPut(ILLEGAL_ACCESS_EXCEPTION);
                    e.printStackTrace();
                } catch (IllegalArgumentException e) {
                    showOutPut(ILLEGAL_ARGUMENT_EXCEPTION);
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    showOutPut(INVOCATION_TARGET_EXCEPTION);
                    e.printStackTrace();
                }
            } else {
                showOutPut(ACTION_NOT_FOUND_MESSAGE);
            }
        }
    }

    public static String getInput()
    {
        String input = null;

        while(mListInput.isEmpty())
        {
            try
            {
                Thread.sleep(1000);
            }
            catch(Exception ex)
            {
            }
        }

        synchronized (mListInput)
        {
            if (!mListInput.isEmpty())
            {
                input = mListInput.get(0);
                mListInput.remove(0);
            }
        }

        return input;
    }
}
