package com.zealens.face.core.internal;

/**
 * Created by yuflh on 2017/4/5.
 */

public interface DrillCallback {
    /**
     * 开始训练后，会触发正式开始（表达底层已经准备好）
     */
    void onBegin();

    /**
     * 训练过程中，一个回合的训练结束，则触发此接口
     *
     * @param drillBoxScore 训练数据
     */
    void onDataChange(TennisBase.DrillBoxScore drillBoxScore);

    /**
     * 速度发生变化时，则触发此接口
     *
     * @param speed 当前球速
     */
    void onSpeedChange(int speed);

    /**
     * 训练结束后，会触发正式结束（表达底层正式停止）
     */
    void onEnd();

    /**
     * 下一回合训练已经准备好，可以开始喂球或者发球
     */
    void onDrillNext();

    /**
     * internal error
     *
     * @param code         error code
     * @param errorMessage
     */
    void onError(int code, String errorMessage);
}
