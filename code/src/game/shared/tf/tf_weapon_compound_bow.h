//=============================================================================
//
// Purpose: A remake of Huntsman from live TF2.
//
//=============================================================================
#ifndef TF_WEAPON_COMPOUND_BOW_H
#define TF_WEAPON_COMPOUND_BOW_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"

#ifdef CLIENT_DLL
#define CTFCompoundBow C_TFCompoundBow
#endif

class CTFCompoundBow : public CTFWeaponBaseGun, public ITFChargeUpWeapon
{
public:
	DECLARE_CLASS( CTFCompoundBow, CTFWeaponBaseGun );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	// Server specific.
#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif

	CTFCompoundBow();

	virtual int		GetWeaponID( void ) const { return TF_WEAPON_COMPOUND_BOW; }

	virtual void	Precache( void );

	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual bool	Deploy( void );
	virtual void	WeaponReset( void );
	virtual void	PrimaryAttack( void );
	virtual void	SecondaryAttack( void );
	virtual void	LowerBow( void );
	virtual void	WeaponIdle( void );
	virtual void	ItemPostFrame( void );

	virtual float	GetProjectileDamage( void );
	virtual float	GetProjectileSpeed( void );
	virtual float	GetProjectileGravity( void );
	virtual void	GetProjectileFireSetup( CTFPlayer *pPlayer, Vector vecOffset, Vector *vecSrc, QAngle *angForward, bool bHitTeammates = true, bool bUseHitboxes = false );
	virtual bool	CalcIsAttackCriticalHelper( void );
	virtual bool    IsFlameArrow( void ) { return m_bFlame; }

	void			LightArrow( void );
	void			Extinguish( void );
	void			FireArrow( void );

	virtual bool	CanReload( void ) { return false; }

#ifdef CLIENT_DLL
	void			CreateMove( float flInputSampleTime, CUserCmd *pCmd, const QAngle &vecOldViewAngles );
	virtual void    OnDataChanged( DataUpdateType_t updateType );
	virtual void	ClientThink( void );
#endif

public:
	// ITFChargeUpWeapon
	virtual float	GetChargeBeginTime( void ) { return m_flChargeBeginTime; }
	virtual float	GetChargeMaxTime( void );
	virtual const char* GetChargeSound( void ) { return NULL; }

private:
	CNetworkVar( float, m_flChargeBeginTime );
	
	CNetworkVar( bool , m_bFlame );

#ifdef CLIENT_DLL
	bool bEmitting;
	bool bFirstPerson;

	EHANDLE m_hFlameEffectHost;
#endif
};

#endif // TF_WEAPON_COMPOUND_BOW_H
