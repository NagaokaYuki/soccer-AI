/*=======================================================================

	ベースゲームエンティティ [baseGameEntity.h]

												長岡佑樹

	すべてのゲームエンティティの共通インターフェイスを定義する基本クラス

=========================================================================*/
#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <iosfwd>
#include "vector2D.h"
#include "geometry.h"
#include "utils.h"

/////////////////////////////////////////////////////////
//
//	構造定義
//
/////////////////////////////////////////////////////////
struct Telegram;

/////////////////////////////////////////////////////////
//
//	BaseGameEntityクラス
//
/////////////////////////////////////////////////////////
class BaseGameEntity
{
public:
	enum {default_entity_type = -1};

private:
	int         m_ID;			// 各エンティティのID
	int         m_iType;		// 各エンティティのタイプ
	bool        m_bTag;		// タグフラグ
	
	// BaseGameEntityがインスタンス化されるたびに
	// この値はタグフラグに更新される
	static int  m_iNextValidID;
	
	// IDが正しく設定されていることを確認するために、
	// 各コンストラクター内で呼び出す
	// IDを設定して次の有効なIDをインクリメントする前に、
	// メソッドに渡された値が次の有効なID以上であることを検証
	void SetID(int val);

protected:
	Vector2D m_vPosition;		// 座標位置
	Vector2D m_vScale;			// 拡大縮小

	// オブジェクトの境界半径の大きさ
	double    m_dBoundingRadius;
	BaseGameEntity(int ID);

public:
	virtual ~BaseGameEntity(){}
	virtual void Update(){}; 
	virtual void Render()=0;
	virtual bool HandleMessage(const Telegram& msg){return false;}
	virtual void Write(std::ostream&  os)const{}
	virtual void Read (std::ifstream& is){}
	
	// 次の有効なIDを取得
	static int   GetNextValidID(){return m_iNextValidID;}
	
	// 次のIDをリセット
	static void  ResetNextValidID(){m_iNextValidID = 0;}

	Vector2D     Pos()const{return m_vPosition;}
	void         SetPos(Vector2D new_pos){m_vPosition = new_pos;}
	
	double       BRadius()const{return m_dBoundingRadius;}
	void         SetBRadius(double r){m_dBoundingRadius = r;}
	int          ID()const{return m_ID;}
	
	bool         IsTagged()const{return m_bTag;}
	void         Tag(){m_bTag = true;}
	void         UnTag(){m_bTag = false;}
	
	Vector2D     Scale()const{return m_vScale;}
	void         SetScale(Vector2D val){m_dBoundingRadius *= MaxOf(val.x, val.y)/MaxOf(m_vScale.x, m_vScale.y); m_vScale = val;}
	void         SetScale(double val){m_dBoundingRadius *= (val/MaxOf(m_vScale.x, m_vScale.y)); m_vScale = Vector2D(val, val);} 
	
	int          EntityType()const{return m_iType;}
	void         SetEntityType(int new_type){m_iType = new_type;}
	
};
      
#endif




