package com.zealens.face.core.internal;

import org.intellij.lang.annotations.MagicConstant;

/**
 * Created by yuflh on 2017/3/20.
 */

public class TennisBase {
    @MagicConstant(intValues = {CourtArea.ANYWHERE,
            CourtArea.A1, CourtArea.A2,
            CourtArea.B1, CourtArea.B2,
            CourtArea.CENTER_A, CourtArea.CENTER_B,
            CourtArea.A, CourtArea.B,
            CourtArea.UNKNOWN, CourtArea.INVALID,})
    public @interface CourtArea {
        int ANYWHERE = 0;
        int A1 = 1;         // Cartesian coordinate --Quadrant 3
        int A2 = 2;         // Cartesian coordinate --Quadrant 2
        int B1 = 3;         // Cartesian coordinate --Quadrant 1
        int B2 = 4;         // Cartesian coordinate --Quadrant 4
        int CENTER_A = 5;
        int CENTER_B = 6;
        int A = 7;
        int B = 8;
        int UNKNOWN = 9;

        int INVALID = -1;
    }

    @MagicConstant(intValues = {PlayModel.MATCH_SINGLE, PlayModel.MATCH_DOUBLE,
            PlayModel.DRILL_SERVE, PlayModel.DRILL_RECEIVE,
            PlayModel.DRILL_FOREHAND_HIT, PlayModel.DRILL_FOREHAND_STRAIGHT,
            PlayModel.DRILL_FOREHAND_SLASH, PlayModel.DRILL_BACKHAND_HIT,
            PlayModel.DRILL_BACKHAND_STRAIGHT, PlayModel.DRILL_BACKHAND_SLASH,
            PlayModel.DRILL_VOLLEY, PlayModel.DRILL_CASUAL,
            PlayModel.DRILL_MULTI_BALL, PlayModel.DRILL_UNKNOWN,})
    public @interface PlayModel {
        int MATCH_SINGLE = 0x100;
        int MATCH_DOUBLE = 0x200;
        int DRILL_SINGLE = 0x400;
        int DRILL_DOUBLE = 0x800;

        int PLAYMODEL_MASK = 0x0F00;

        int DRILL_SERVE = 1;                            // serve
        int DRILL_RECEIVE = 2;                          // receive
        int DRILL_FOREHAND_HIT = 3;
        int DRILL_FOREHAND_STRAIGHT = 4;                // forehand down the line
        int DRILL_FOREHAND_SLASH = 5;                   // forehand cross the court
        int DRILL_BACKHAND_HIT = 6;
        int DRILL_BACKHAND_STRAIGHT = 7;                // backhand down the line
        int DRILL_BACKHAND_SLASH = 8;                   // backhand cross the court
        int DRILL_VOLLEY = 9;
        int DRILL_CASUAL = 10;                           // free style
        int DRILL_MULTI_BALL = 11;
        int DRILL_UNKNOWN = 100;

        int DRILL_MODEL_MASK = 0xFF;
    }

    @MagicConstant(intValues = {BallPower.A, BallPower.B,})
    public @interface BallPower {
        int A = 1;
        int B = 2;
    }

    @MagicConstant(intValues = {Player.PLAYER_INVALID, Player.PLAYER_A, Player.PLAYER_B,})
    public @interface Player {
        int PLAYER_INVALID = 0;
        int PLAYER_A = 1;
        int PLAYER_B = 2;
    }

    @MagicConstant(intValues = {PlayerServe.ACE, PlayerServe.FIRST, PlayerServe.SECOND,})
    public @interface PlayerServe {
        int ACE = 0;
        int FIRST = 1;
        int SECOND = 2;
    }

    @MagicConstant(intValues = {DrillResult.UNKNOWN, DrillResult.INVALID, DrillResult.IN, DrillResult.OUT, DrillResult.NOHIT, DrillResult.MULTIHIT})
    public @interface DrillResult {
        int UNKNOWN = 0;
        int INVALID = 1;
        int IN = 2;
        int OUT = 3;
        int NOHIT = 4;
        int MULTIHIT = 5;
    }

    @MagicConstant(intValues = {DrillDifficulty.UNKNOWN, DrillDifficulty.EASY, DrillDifficulty.MEDIUM, DrillDifficulty.DIFFICULT})
    public @interface DrillDifficulty {
        int UNKNOWN = 0;
        int EASY = 1;
        int MEDIUM = 2;
        int DIFFICULT = 3;
    }

    public static class CourtSize {
        // unit : mm
        public int singleX;// single court axis x scope
        public int doubleX;// double axis x scope
        public int courtX;// camera axis x scope
        public int singleY;// single court axis y scope
        public int doubleY;// double axis y scope
        public int courtY;// camera axis y scope

        public int severX;                // serve line axis x coordinate, defaultValue = 6401
        public int baselineX;              // base line axis x coordinate
        public int centerLineWidth;        // from50 to100mm, defaultValue = 50mm

        public CourtSize() {
            singleX = 11887;
            doubleX = 11887;
            singleY = 4115;
            doubleY = 5487;
            courtX = 13887;
            courtY = 7487;

            severX = 6401;
            baselineX = 11887;
            centerLineWidth = 50;
        }
    }

    public static class Ball {
        public int x;// x coordinate, unit:mm
        public int y;// y coordinate, unit:mm
        public int z;// z coordinate, unit:mm
        public int speed;
        public long timestamp;//unit: milliseconds

        @Override
        public String toString() {
            return "Ball{" +
                    "x=" + x +
                    ", y=" + y +
                    ", z=" + z +
                    ", speed=" + speed +
                    ", timestamp=" + timestamp +
                    '}';
        }
    }

    public static class InitParam {
        @PlayModel
        public int playModel;
        @CourtArea
        public int servePosition;
        public CourtSize courtSize;

        public String appPath;// path of application
        public String cfgPath;// configuration path
        public String tmpPath;// temp path, for orbit track photos etc.

        public InitParam() {
            playModel = PlayModel.MATCH_SINGLE;// default play mode: serve train
            servePosition = CourtArea.A1;// default serve area: invalid
            courtSize = new CourtSize();
        }
    }

    public static class MatchBoxScore {
        public int score;
        public int ace;
        public int doubleFaultsNum;
        public int n1stServeTotalNum;          // 1st total times
        public int n1stServeSNum;              // 1st serve successfully times
        public int n1stSocreSNum;              // 1st win score times
        public int n2ndServeTotalNum;          // 2nd total times
        public int n2ndServeSNum;              // 2nd serve successfully times
        public int n2ndSocreSNum;              // 2nd win score times
        public int winners;
        public int speedMax;                   // max speed of serve
        public int speedSum;                   // sum of serve speed
        public int hitTotalNum;                // hit number, for video record above 20 hits

        public int hitNumInside;
        public int hitNumBaseline;
        public int hitNumOutside;

        // 1stServe
        public int area11stInsideNum;          // 1stArea1InsideNum
        public int area11stOutsideNum;         // 1stArea1OutsideNum
        public int area21stInsideNum;          // 1stArea2InsideNum
        public int area21stOutsideNum;         // 1stArea2OutsideNum

        // 2ndServe
        public int area12stInsideNum;          // 2stArea1InsideNum
        public int area12stOutsideNum;         // 2stArea1OutsideNum
        public int area22ndInsideNum;          // 2ndArea2InsideNum
        public int area22stOutsideNum;         // 2stArea2OutsideNum

        @Override
        public String toString() {
            return "MatchBoxScore{" +
                    "score=" + score +
                    ", ace=" + ace +
                    ", doubleFaultsNum=" + doubleFaultsNum +
                    ", n1stServeTotalNum=" + n1stServeTotalNum +
                    ", n1stServeSNum=" + n1stServeSNum +
                    ", n1stSocreSNum=" + n1stSocreSNum +
                    ", n2ndServeTotalNum=" + n2ndServeTotalNum +
                    ", n2ndServeSNum=" + n2ndServeSNum +
                    ", n2ndSocreSNum=" + n2ndSocreSNum +
                    ", winners=" + winners +
                    ", speedMax=" + speedMax +
                    ", speedSum=" + speedSum +
                    ", hitTotalNum=" + hitTotalNum +
                    ", hitNumInside=" + hitNumInside +
                    ", hitNumBaseline=" + hitNumBaseline +
                    ", hitNumOutside=" + hitNumOutside +
                    ", area11stInsideNum=" + area11stInsideNum +
                    ", area11stOutsideNum=" + area11stOutsideNum +
                    ", area21stInsideNum=" + area21stInsideNum +
                    ", area21stOutsideNum=" + area21stOutsideNum +
                    ", area12stInsideNum=" + area12stInsideNum +
                    ", area12stOutsideNum=" + area12stOutsideNum +
                    ", area22ndInsideNum=" + area22ndInsideNum +
                    ", area22stOutsideNum=" + area22stOutsideNum +
                    '}';
        }
    }

    public static class BallPoint {
        public int x;
        public int y;
        public boolean in;

        @Override
        public String toString() {
            return "BallPoint{" +
                    "x=" + x +
                    ", y=" + y +
                    ", in=" + in +
                    '}';
        }
    }

    public static class DrillBoxScore {
        public int totalTimes;
        public int successfulTimes;
        public int maxSpeed;
        public int averageSpeed;
        public int successRatio;
        public int netTimes;
        public int outTimes;
        @DrillResult
        public int lastResult;
        public int speed;
        public int hitPointsNum;
        public int downPointsNum;
        public BallPoint[] ballPointHit;
        public BallPoint[] ballPointDown;

        public static String toString(Object[] a, int length) {
            if (a == null)
                return "null";

            int iMax = Math.min(a.length, length) - 1;
            if (iMax == -1)
                return "[]";

            StringBuilder b = new StringBuilder();
            b.append('[');
            for (int i = 0; ; i++) {
                b.append(String.valueOf(a[i]));
                if (i == iMax)
                    return b.append(']').toString();
                b.append(", ");
            }
        }

        @Override
        public String toString() {
            return "DrillBoxScore{" +
                    "totalTimes=" + totalTimes +
                    ", successfulTimes=" + successfulTimes +
                    ", maxSpeed=" + maxSpeed +
                    ", averageSpeed=" + averageSpeed +
                    ", successRatio=" + successRatio +
                    ", netTimes=" + netTimes +
                    ", outTimes=" + outTimes +
                    ", lastResult=" + lastResult +
                    ", speed=" + speed +
                    ", downPointsNum=" + downPointsNum +
                    ", hitPointsNum=" + hitPointsNum +
                    ", ballPointHit=" + DrillBoxScore.toString(ballPointHit, hitPointsNum) +
                    ", ballPointDown=" + DrillBoxScore.toString(ballPointDown, downPointsNum) +
                    '}';
        }
    }
}
