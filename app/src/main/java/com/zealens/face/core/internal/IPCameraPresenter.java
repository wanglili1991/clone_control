package com.zealens.face.core.internal;

import com.zealens.face.domain.DomainConst;

/**
 * Colorful IPCamera record interface
 */
public interface IPCameraPresenter {
    /**
     * Camera initialize
     *
     * @param ip       Ip Address
     * @param port     port, defaultValue = 80
     * @param userName userName, defaultValue = admin
     * @param password password, defaultValue = admin
     * @return true for success
     * @see DomainConst#IP_CAMERA_DEFAULT_PORT
     * @see DomainConst#IP_CAMERA_DEFAULT_USERNAME
     * @see DomainConst#IP_CAMERA_DEFAULT_PASSWORD
     */
    boolean init(final String tmpPath, final String ip, short port, final String userName, final String password);

    void dispose();


    /**
     * start to record, store the video in the assigned path
     *
     * @param savePath path to save the record video
     * @return true for success
     */
    boolean startRecord(final String savePath);

    /**
     * @return true for success
     */
    boolean stopRecord();
}
