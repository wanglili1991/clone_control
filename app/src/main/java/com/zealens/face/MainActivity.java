package com.zealens.face;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.support.annotation.IdRes;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;

import com.zealens.face.core.internal.BoutCallback;
import com.zealens.face.core.internal.DrillCallback;
import com.zealens.face.core.internal.TennisBase;
import com.zealens.face.core.internal.TennisBout;
import com.zealens.face.core.internal.TennisDrill;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

import static junit.framework.Assert.assertTrue;

public class MainActivity extends AppCompatActivity implements BoutCallback, DrillCallback {
    private static final int MSG_RUN_ENGINE = 0x110;
    private static final int MSG_NEXT_DRILLL = 0x120;

    private static final int DEFAULT_DRILL_START_CASE = 1;

    final String TAG = MainActivity.class.getSimpleName();
    private int mIndexCase = DEFAULT_DRILL_START_CASE;
    private TennisBout mTennisBout;
    private TennisDrill mTennisDrill;
    private TennisBase.InitParam mInitParam;
    private HandlerThread mRunMsgThread;
    private Handler mRunMsgHandler;
    @TennisBase.PlayModel
    private int mPlayModel = TennisBase.PlayModel.MATCH_SINGLE;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setInitParam();
        initBackThread();

        RadioGroup group = (RadioGroup) this.findViewById(R.id.radioGroup);
        group.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, @IdRes int checkedId) {
                int radioButtonId = group.getCheckedRadioButtonId();
                if (radioButtonId == R.id.radioMatch) {
                    mPlayModel = TennisBase.PlayModel.MATCH_SINGLE;
                } else {
                    mPlayModel = TennisBase.PlayModel.DRILL_SINGLE;
                }
            }
        });

        Button button = (Button) findViewById(R.id.run);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String eventID = ((EditText) findViewById(R.id.eventID)).getText().toString().trim();
                if (eventID.length() != 4)
                    return;

                eventID = eventID.toUpperCase();
                if ((mPlayModel & TennisBase.PlayModel.PLAYMODEL_MASK) == TennisBase.PlayModel.MATCH_SINGLE) {
                    if (eventID.getBytes()[0] != 'D')
                        return;

                    mPlayModel = TennisBase.PlayModel.MATCH_SINGLE;
                } else {
                    byte type = eventID.getBytes()[0];
                    if (type == 'D')
                        return;

                    mPlayModel = TennisBase.PlayModel.DRILL_SINGLE;
                    switch (type) {
                        case 'A':
                            mPlayModel |= TennisBase.PlayModel.DRILL_SERVE;
                            break;
                        case 'B':
                            mPlayModel |= TennisBase.PlayModel.DRILL_FOREHAND_HIT;
                            break;
                        case 'C':
                            mPlayModel |= TennisBase.PlayModel.DRILL_RECEIVE;
                            break;
                        case 'E':
                            mPlayModel |= TennisBase.PlayModel.DRILL_FOREHAND_STRAIGHT;
                            break;
                        case 'F':
                            mPlayModel |= TennisBase.PlayModel.DRILL_FOREHAND_SLASH;
                            break;
                        case 'G':
                            mPlayModel |= TennisBase.PlayModel.DRILL_VOLLEY;
                            break;
                    }

                    String indexCase = eventID.substring(1);
                    mIndexCase = Integer.valueOf(indexCase) + 1;
                }

                IniFile file = new IniFile(new File(mInitParam.cfgPath + "/config.cfg"));
                file.set("mixed", "event_id", eventID);
                file.save();

                mRunMsgHandler.sendEmptyMessage(MSG_RUN_ENGINE);
            }
        });
    }

    void setInitParam() {
        String appPath = getApplicationInfo().nativeLibraryDir;

        File file = getFilesDir();
        final String configPath = file.getPath() + "/config";
        if (!copyFilesAssets(this, "config", configPath))
            return;

        String tmpPath = file.getPath() + "/tmp";
        if (!copyFilesAssets(this, "tmp", tmpPath))
            return;

        mInitParam = new TennisBase.InitParam();
        mInitParam.appPath = appPath + "/";
        mInitParam.cfgPath = configPath + "/";
        mInitParam.tmpPath = tmpPath + "/";
    }

    private void initBackThread() {
        mRunMsgThread = new HandlerThread("run-message-coming");
        mRunMsgThread.start();
        mRunMsgHandler = new Handler(mRunMsgThread.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case MSG_RUN_ENGINE: {
                        if (mTennisDrill != null) {
                            mTennisDrill.dispose();
                            mTennisDrill = null;
                        }

                        if (mTennisBout != null) {
                            mTennisBout.dispose();
                            ;
                            mTennisBout = null;
                        }

                        if ((mPlayModel & TennisBase.PlayModel.PLAYMODEL_MASK) == TennisBase.PlayModel.MATCH_SINGLE) {
                            mTennisBout = new TennisBout();

                            mInitParam.playModel = mPlayModel;

                            assertTrue(mTennisBout.init(mInitParam, MainActivity.this));
                            assertTrue(mTennisBout.ballPower(TennisBase.BallPower.A));
                            assertTrue(mTennisBout.start());
                        } else {
                            mTennisDrill = new TennisDrill();

                            mInitParam.playModel = mPlayModel;
                            assertTrue(mTennisDrill.init(mInitParam, MainActivity.this, TennisBase.DrillDifficulty.EASY));
                            assertTrue(mTennisDrill.start());
                        }
                    }
                    break;
                    case MSG_NEXT_DRILLL: {
                        if (mIndexCase == -1)
                            break;

                        IniFile file = new IniFile(new File(mInitParam.cfgPath + "/config.cfg"));
                        file.set("mixed", "event_id", getTennisDrillEventID());
                        file.save();

                        assertTrue(mTennisDrill.restart(TennisBase.CourtArea.A1));
                    }
                    break;
                    default:
                        break;
                }

            }
        };
    }

    String getTennisDrillEventID() {
        String eventID = null;
        switch (mInitParam.playModel & TennisBase.PlayModel.DRILL_MODEL_MASK) {
            case TennisBase.PlayModel.DRILL_SERVE: {
                eventID = "A00";
                if (mIndexCase >= 10) {
                    eventID = "A0";
                }

                eventID += mIndexCase;
                if (++mIndexCase == 17) {
                    mIndexCase = -1;
                }

                break;
            }
            case TennisBase.PlayModel.DRILL_RECEIVE: {
                eventID = "C00";
                if (mIndexCase >= 10) {
                    eventID = "C0";
                }

                eventID += mIndexCase;
                if (++mIndexCase == 19) {
                    mIndexCase = -1;
                }

                break;
            }
            case TennisBase.PlayModel.DRILL_FOREHAND_HIT:
            case TennisBase.PlayModel.DRILL_BACKHAND_HIT: {
                eventID = "B00";
                if (mIndexCase >= 10) {
                    eventID = "B0";
                }

                eventID += mIndexCase;
                if (++mIndexCase == 17) {
                    mIndexCase = -1;
                }

                break;
            }
            case TennisBase.PlayModel.DRILL_FOREHAND_STRAIGHT:
            case TennisBase.PlayModel.DRILL_BACKHAND_STRAIGHT: {
                eventID = "E00";
                if (mIndexCase >= 10)
                    eventID = "E0";

                eventID += mIndexCase;
                if (++mIndexCase == 17) {
                    mIndexCase = -1;
                }

                break;
            }
            case TennisBase.PlayModel.DRILL_FOREHAND_SLASH:
            case TennisBase.PlayModel.DRILL_BACKHAND_SLASH: {
                eventID = "F00";
                if (mIndexCase >= 10) {
                    eventID = "F0";
                }

                eventID += mIndexCase;
                if (++mIndexCase == 17) {
                    mIndexCase = -1;
                }

                break;
            }
            case TennisBase.PlayModel.DRILL_VOLLEY: {
                eventID = "G00";
                if (mIndexCase >= 10) {
                    eventID = "G0";
                }

                eventID += mIndexCase;
                if (++mIndexCase == 18) {
                    mIndexCase = -1;
                }

                break;
            }
            default:
                break;
        }

        return eventID;
    }

    /**
     * 从assets目录中复制整个文件夹内容
     *
     * @param context Context 使用CopyFiles类的Activity
     * @param oldPath String  原文件路径  如：/aa
     * @param newPath String  复制后路径  如：xx:/bb/cc
     */
    /**
     * @param context Context 使用CopyFiles类的Activity
     * @param oldPath String  原文件路径  如：/aa
     * @param newPath String  复制后路径  如：xx:/bb/cc
     * @return 成功返回 true， 失败返回 false
     */
    public boolean copyFilesAssets(Context context, String oldPath, String newPath) {
        try {
            String fileNames[] = context.getAssets().list(oldPath);
            if (fileNames.length > 0) {
                File file = new File(newPath);
                file.mkdirs();
                for (String fileName : fileNames) {
                    copyFilesAssets(context, oldPath + "/" + fileName, newPath + "/" + fileName);
                }
            } else {
                InputStream is = context.getAssets().open(oldPath);
                FileOutputStream fos = new FileOutputStream(new File(newPath));
                byte[] buffer = new byte[8096];
                int byteCount = 0;
                while ((byteCount = is.read(buffer)) != -1) {
                    fos.write(buffer, 0, byteCount);
                }
                fos.flush();
                is.close();
                fos.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    @Override
    public void onBall(TennisBase.Ball ball) {
        Log.w(TAG, "onBall ball:" + ball.toString());
    }

    @Override
    public void onBallAce(@TennisBase.Player int playerServe) {
        Log.w(TAG, "onBallAce playerServe:" + playerServe);
    }

    @Override
    public void onServe(TennisBase.Ball ballServe) {
        Log.w(TAG, "onServe ballServe:" + ballServe.toString());
    }

    @Override
    public void onServeFailed() {
        Log.e(TAG, "onServeFailed");
    }

    @Override
    public void onServeSuccessful() {
        Log.w(TAG, "onServeSuccessful");
    }

    @Override
    public void onServeTouchDownPosition(@TennisBase.Player int player, @TennisBase.PlayerServe int serveType, TennisBase.Ball ballServeTouchDown) {
        Log.w(TAG, "onServeTouchDownPosition player:" + player + " serveType:" + serveType + " ballServeTouchDown:" + ballServeTouchDown);
    }

    @Override
    public void onHitPosition(@TennisBase.Player int playerHit, TennisBase.Ball ballHit) {
        Log.w(TAG, "onHitPosition playerHit:" + playerHit + " ballHit:" + ballHit);
    }

    @Override
    public void onAddScore(@TennisBase.Player int player, TennisBase.MatchBoxScore matchBoxScore1, TennisBase.MatchBoxScore matchBoxScore2, TennisBase.Ball ball) {
        Log.e(TAG, "onAddScore player:" + player + " matchBoxScore1:" + matchBoxScore1 + " matchBoxScore2:" + matchBoxScore2 + " ball:" + ball);
    }

    @Override
    public void onBegin() {
        Log.e(TAG, "onBegin");
    }

    @Override
    public void onDataChange(TennisBase.DrillBoxScore drillBoxScore) {
        mRunMsgHandler.sendEmptyMessage(MSG_NEXT_DRILLL);
        Log.e(TAG, "onDataChange drillBoxScore:" + drillBoxScore);
    }

    @Override
    public void onSpeedChange(int speed) {
        Log.e(TAG, "onSpeedChange speed:" + speed);
    }

    @Override
    public void onEnd() {
        Log.e(TAG, "onEnd");
    }

    @Override
    public void onDrillNext() {
        Log.e(TAG, "onDrillNext");
    }

    @Override
    public void onError(int code, String errorMessage) {
//        mRunMsgHandler.sendEmptyMessage(MSG_RUN_ENGINE);
        Log.e(TAG, "onError code:" + code + " errorMessage:" + errorMessage);
    }
}
