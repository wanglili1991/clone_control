package com.zealens.face;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.zealens.face.core.internal.IPCamera;

import java.io.File;

/**
 * Created by Songkang on 4/1/17.
 * Email:songkang@tiantianld.com
 * Tips: This Class File for Android developer use Ipcamera SDK demo.
 */

public class CameraDemo extends Activity {
    private static final String ip="192.168.1.198";
    private static final short port=0;
    private static final String username="admin";
    private static final String password="123456";

    private IPCamera mHandler;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ipcamera_demo);
        mHandler = new IPCamera();

        File file = getFilesDir();
        final String tmpPath = file.getPath() + "/tmp/";

        if (!mHandler.init(tmpPath, ip, port, username, password))
            return;

        Button startButton = (Button)findViewById(R.id.start);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!mHandler.startRecord(tmpPath))
                    return;
            }
        });

        Button stopButton = (Button)findViewById(R.id.stop);
        stopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mHandler.stopRecord();
            }
        });
    }
}
