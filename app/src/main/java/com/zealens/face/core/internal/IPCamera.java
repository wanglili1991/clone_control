package com.zealens.face.core.internal;

import com.zealens.face.domain.DomainConst;

/**
 * 彩色相机录制接口
 */
public class IPCamera implements IPCameraPresenter {
    static {
        System.loadLibrary("ipcamera_sdk");
    }

    /**
     * 相机初始化函数
     *
     * @param tmpPath 临时路径
     * @param ip       相机IP地址
     * @param port     相机端口，默认：80
     * @param userName 相机登陆用户名，默认：admin
     * @param password 相机登陆密码：默认：admin
     * @return 初始化成功，返回 true；否则，返回 false
     * @see DomainConst#IP_CAMERA_DEFAULT_PORT
     * @see DomainConst#IP_CAMERA_DEFAULT_USERNAME
     * @see DomainConst#IP_CAMERA_DEFAULT_PASSWORD
     */
    public native boolean init(String tmpPath, String ip, short port, String userName, String password);

    /**
     * 相机释放函数
     */
    public native void dispose();

    /**
     *  开始录制录像，并保存在指定目录
     * @param savePath  录像保存路径，注意：不能使用相同路径保存录像
     * @return  录制成功，返回 true；否则，返回 false
     */
    public native boolean startRecord(String savePath);

    /**
     *  停止录制录像
     * @return  停止录制成功，返回 true；否则，返回 false
     */
    public native boolean stopRecord();
}
