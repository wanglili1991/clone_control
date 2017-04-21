package com.zealens.face.core.internal;

/**
 * Created on 2017/3/23
 * in BlaBla by Kyle
 */

public interface BoutCallback {
    /**
     * Position of ball
     *
     * @param ball
     */
    void onBall(TennisBase.Ball ball);

    /**
     * ace
     *
     * @param playerServe the serve player
     */
    void onBallAce(@TennisBase.Player int playerServe);

    /**
     * serve position
     *
     * @param ballServe serve ball position
     */
    void onServe(TennisBase.Ball ballServe);

    void onServeFailed();

    void onServeSuccessful();

    /**
     * serve ball touch position on ground
     *
     * @param player             server
     * @param serveType          serve type
     * @param ballServeTouchDown serve ball touch position on ground
     */
    void onServeTouchDownPosition(@TennisBase.Player int player, @TennisBase.PlayerServe int serveType, TennisBase.Ball ballServeTouchDown);

    /**
     * position where hit ball
     *
     * @param playerHit
     * @param ballHit   ball position
     */
    void onHitPosition(@TennisBase.Player int playerHit, TennisBase.Ball ballHit);

    /**
     * technology statistics of one bout
     *
     * @param player
     * @param matchBoxScoreA technology statistics of side A player
     * @param matchBoxScoreB technology statistics of side B player
     * @param ball      ball
     */
    void onAddScore(@TennisBase.Player int player, TennisBase.MatchBoxScore matchBoxScoreA, TennisBase.MatchBoxScore matchBoxScoreB, TennisBase.Ball ball);

    /**
     * internal error
     *
     * @param code         error code
     * @param errorMessage
     */
    void onError(int code, String errorMessage);
}

