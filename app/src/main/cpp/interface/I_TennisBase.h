#ifndef __INTERFACE_TENNISBASE_H__
#define __INTERFACE_TENNISBASE_H__

#include <functional>

typedef long long ll;

#if FALSE
#define TENNIS_STDCALL __attribute__(( __stdcall ))
#else
#define TENNIS_STDCALL
#endif


//TODO:
//	CourtSize �򳡳ߴ��ʼ��������Ӧ��ÿ����������ʱ�������ļ��У�����Ӧ��ͬ�򳡵ı仯������CourtSizeʱһ�㳡�ݵ�Ĭ��ֵ��

// OLD ��һ�µ���
// ������pos ����λ�ã�1��2ΪA����3��4ΪB��
// ѵ����pos ����λ�ã�0Ϊȫ����1ΪA����2ΪB��
// NEW ���£�
enum CourtArea
{
	COURTAREA_ANYWHERE,	// ����λ��
	COURTAREA_A1,		// A1�棬��׼������ϵ����������
	COURTAREA_A2,		// A2�棬��׼������ϵ���ڶ�����
	COURTAREA_B1,		// B1�棬��׼������ϵ����һ����
	COURTAREA_B2,		// B2�棬��׼������ϵ����������
	COURTAREA_CENTER_A,	// A�� �м�
	COURTAREA_CENTER_B,	// B�� �м�	
	COURTAREA_A,
	COURTAREA_B,
	COURTAREA_UNKNOWN,	// δ֪
		
	COURTAREA_INVALID = -1, 	// �Ƿ�λ��
};

enum PlayModel
{
    PLAYMODEL_MATCH_SINGLE = 0x100,                            // ��������
    PLAYMODEL_MATCH_DOUBLE = 0x200,                            // ����˫��
    PLAYMODEL_DRILL_SINGLE = 0x400,                            // ѵ������
    PLAYMODEL_DRILL_DOUBLE = 0x800,                            // ѵ��˫��

    PLAYMODEL_MASK = 0x0F00,

    PLAYMODEL_DRILL_SERVE = 1,                            // ����
    PLAYMODEL_DRILL_RECEIVE = 2,                          // �ӷ���
    PLAYMODEL_DRILL_FOREHAND_HIT = 3,                     // ���ֻ���
    PLAYMODEL_DRILL_FOREHAND_STRAIGHT = 4,                // ����ֱ��
    PLAYMODEL_DRILL_FOREHAND_SLASH = 5,                   // ����б��
    PLAYMODEL_DRILL_BACKHAND_HIT = 6,                     // ���ֻ���
    PLAYMODEL_DRILL_BACKHAND_STRAIGHT = 7,                // ����ֱ��
    PLAYMODEL_DRILL_BACKHAND_SLASH = 8,                   // ����б��
    PLAYMODEL_DRILL_VOLLEY = 9,                           // �ػ�
    PLAYMODEL_DRILL_CASUAL = 10,                           // �����
    PLAYMODEL_DRILL_MULTI_BALL = 11,                      // ����
    PLAYMODEL_DRILL_UNKNOWN = 100,                         // δ֪

    PLAYMODEL_DRILL_MODEL_MASK = 0xFF,
};

enum BallStat
{
	BALLSTAT_NULL = 0,
	BALLSTAT_HIT = 1,                                // ����
	BALLSTAT_SERVE = 2,                              // ����
	BALLSTAT_TOUCH_DOWN = 3,                         // ���
	BALLSTAT_TOUCH_NET = 4,                          // ������
	BALLSTAT_TOUCH_ANY = 5,                          // ������
	BALLSTAT_OUT = 6,                                // �ɳ���Ұ
	BALLSTAT_NON_UNIQUENESS = 7,                     // ��Ψһ
	BALLSTAT_ERROR = 8,                              // �쳣
	BALLSTAT_IN = 9,                                 // ������Ұ
	BALLSTAT_HIT_GUESS = 10,                         // �²����
	BALLSTAT_ERROR_HIT = 11,                         // �쳣����
	BALLSTAT_WAITING = 12                            // �ȴ�״̬
};

enum BallPower
{
	BALLPOWER_INVALID = 0,
	BALLPOWER_A = 1,		// A����Ȩ
	BALLPOWER_B = 2,		// B����Ȩ
};

enum BallDirection
{
	BALLDIRECTION_INVALID = 0,
	BALLDIRECTION_A = 1,	// ����A��
	BALLDIRECTION_B = 2,	// ����B��
	BALLDIRECTION_UNKNOWN = 3,	// ��δ֪����
};

enum Player
{
	PLAYER_INVALID = 0,
	PLAYER_A = 1,			// A��ѡ��
	PLAYER_B = 2,			// B��ѡ��
};

enum PlayerServe
{
	PALYERSERVE_ACE = 0,		// ACE
	PALYERSERVE_FIRST = 1,		// һ��
	PLAYERSERVE_SECOND = 2,		// ����
};

#pragma pack(push, 4)
struct Ball
{
    int x;                                            // ������x����λ������
    int y;											  // ������y����λ������
    int z;											  // ������z����λ������
    int speed;										  // ���ٶ�
    ll  timestamp;                                    // ��ǰʱ���,��λ������
};

struct CourtSize
{
    // unit : mm
    int singleX;               // ���˳���x�᷶Χ
    int doubleX;               // ˫�˳���x�᷶Χ
    int courtX;                // ����x�᷶Χ
    int singleY;                // ���˳���y�᷶Χ
    int doubleY;                // ˫�˳���y�᷶Χ
    int courtY;                 // ����y�᷶Χ

    int severX;                // ������ x ���꣬Ĭ��ֵ6401
    int baselineX;              // ���� x ����
    int centerLineWidth;        // �����ߵĿ�ȣ�ͨ����50-100mm��Ĭ����50mm
};

struct InitParam
{
    PlayModel playModel;             // ����ģʽ
	CourtArea servePosition;           // ����λ�û�ѵ����������λ��
    CourtSize courtSize;                // �򳡴�С

    std::string strAppPath;            // ��������Ŀ¼����"/"��β
    std::string strCfgPath;            // �����ļ�Ŀ¼����"/"��β
    std::string strTmpPath;            // ��ʱĿ¼���켣����ͼƬ�ȣ�
};

// ������ײ㽻���Ľӿڣ������Ը��ģ�
// ����Ҫ�޸ģ�����Ҫ��ײ��Ŷ�Э��
///////////////////////////////////////////////////the interface///////////////////////////////////////////////////////////////////////////////////
struct I_TennisBase
{
public:			   

	/**
	* ���ӵײ����ü���
	* @return ���� �������ü���
	*/
	virtual unsigned int TENNIS_STDCALL addRef() = 0;
	//{
	//	return ++m_uRefCount;
	//}

	/**
	* �ͷŵײ����ü����������ü������� 0 ʱ���ͷŴ˶���
	* @return ���� �������ü���
	*/
	virtual unsigned int TENNIS_STDCALL releaseRef() = 0;
	//{
	//	if (--m_uRefCount == 0)
	//	{
	//		delete this;
	//		return 0;
	//	}

	//	return m_uRefCount;
	//}


	/**
	* ��ʼ��
	* @param initParam ��ʼ������
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL init( const InitParam &initParam ) = 0; 


	/**
	* ������������ͷ����׽��Ƶ������
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL start()   = 0;              


	/**
	* ֪ͨ�ײ���ʱֹͣ���ݴ���
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL pause()   = 0;                           


	/**
     * ֪ͨ���¿�ʼ��������
     * @return �ɹ����� true��ʧ�ܷ��� false
     */
    virtual bool TENNIS_STDCALL resume()  = 0;  


	/**
	* ��ֹ�ײ����
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL stop()    = 0;       


	/**
	* ��ȡ�ײ�����
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL runOnce() = 0;                             


	/**
	* ��ʼһ���µĻغ�
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
	virtual bool TENNIS_STDCALL restart(CourtArea serveArea) = 0;   

protected:
	virtual ~I_TennisBase()
	{

	}

public:
	/**
	* ÿ10ms���һ�������Ϣ
	*/
	std::function<void(BallStat ballStat, const Ball &ball)> onBall;


	/**
	* ��������³��ֲ����޸��쳣����������ʱ�Ҳ�������ͷ,�����������豸���ϵ�
	*/
	std::function<void(int errorCode, const char *pErrorMessage)> onError;


	/**
	 *  �̴߳������˳�ʱ���ص�
	 */
	std::function<void(bool isExit)> onThread;


	/**
	* �ײ��Ѿ�׼���ã����Կ�ʼ��һ��ѵ��
	*/
	std::function<void()> onDrillNext;

//private:
//	unsigned int m_uRefCount;
};
///////////////////////////////////////////////////the interface///////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)


typedef I_TennisBase *(*CREATE_TENNISBASE_INTERFACE)();

#endif // __INTERFACE_BASECONSTRUCT_H__