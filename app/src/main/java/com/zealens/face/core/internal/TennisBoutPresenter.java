package com.zealens.face.core.internal;

/**
 * one set control engine interface
 */
public interface TennisBoutPresenter {
    /**
     * init the internal hardware
     * @return true for success
     */
    boolean init(TennisBase.InitParam initParam, BoutCallback pCallback);

    /**
     * set ball power
     *
     * @param ballPower
     * @return true for success
     */
    boolean ballPower(@TennisBase.BallPower int ballPower);

    /**
     * start all cameras for capture videos, analyze video
     *
     * @return true for success
     */
    boolean start();

    /**
     * @return true for success
     */
    boolean pause();

    /**
     * @return true for success
     */
    boolean resume();

    /**
     * stop internal service
     *
     * @return true for success
     */
    boolean stop();

    /**
     * test internal data
     *
     * @return true for success
     */
    boolean runOnce();

    /**
     * restart another set, avoiding the re-initialize operation
     *
     * @return true for success
     */
    boolean restart(@TennisBase.CourtArea int serveAre);
}
