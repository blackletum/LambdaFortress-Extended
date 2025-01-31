//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======
//
//
//=============================================================================
#ifndef TF_WEAPON_FLAMETHROWER_H
#define TF_WEAPON_FLAMETHROWER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"
#include "tf_weaponbase_rocket.h"
#include "tf_weapon_grenade_pipebomb.h"
#include "tf_flame.h"

// Client specific.
#ifdef CLIENT_DLL
	#include "particlemgr.h"
	#include "particle_util.h"
	#include "particles_simple.h"
	#include "c_tf_projectile_rocket.h"

	#define CTFFlameThrower C_TFFlameThrower
	#define CTFFlameRocket C_TFFlameRocket
#else
	#include "tf_projectile_rocket.h"
	#include "baseentity.h"
#endif

enum FlameThrowerState_t
{
	// Firing states.
	FT_STATE_IDLE = 0,
	FT_STATE_STARTFIRING,
	FT_STATE_FIRING,
	FT_STATE_AIRBLASTING
};

//=========================================================
// Flamethrower Weapon
//=========================================================
class CTFFlameThrower : public CTFWeaponBaseGun
{
	DECLARE_CLASS( CTFFlameThrower, CTFWeaponBaseGun );
public:
	DECLARE_DATADESC();
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CTFFlameThrower();
	~CTFFlameThrower();

	virtual void	Spawn( void );
	virtual void	Precache( void );

	virtual int		GetWeaponID( void ) const { return TF_WEAPON_FLAMETHROWER; }

	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual void	ItemPostFrame( void );
	virtual void	PrimaryAttack();
	virtual void	SecondaryAttack();

	virtual bool	Lower( void );
	virtual void	WeaponReset( void );

	virtual void	DestroySounds( void );

	Vector			GetVisualMuzzlePos();
	Vector			GetFlameOriginPos();

	bool			IsBehindAndFacingTarget( CBaseEntity *pTarget );
	// Client specific.
#if defined( CLIENT_DLL )
	virtual bool	Deploy( void );

	virtual void	OnDataChanged(DataUpdateType_t updateType);
	virtual void	UpdateOnRemove( void );
	virtual void	SetDormant( bool bDormant );

	//	Start/stop flame sound and particle effects
	void			StartFlame();
	void			StopFlame( bool bAbrupt = false );

	void			RestartParticleEffect();	

	// constant pilot light sound
	void 			StartPilotLight();
	void 			StopPilotLight();

	// Server specific.
#else

	void			SetHitTarget( void );
	void			HitTargetThink( void );

#endif

private:
	Vector GetMuzzlePosHelper( bool bVisualPos );
	CNetworkVar( int, m_iWeaponState );
	CNetworkVar( bool, m_bCritFire );
	CNetworkVar( bool, m_bHitTarget );

	float m_flStartFiringTime;
	float m_flNextPrimaryAttackAnim;

	int			m_iParticleWaterLevel;
	float		m_flAmmoUseRemainder;

	// Don't need network this as mini-crits don't use special particle effects
	bool m_bMiniCritFire;

#if defined( CLIENT_DLL )
	CSoundPatch	*m_pFiringStartSound;

	CSoundPatch	*m_pFiringLoop;
	bool		m_bFiringLoopCritical;

	CNewParticleEffect *m_pFlameEffect;
	EHANDLE		m_hFlameEffectHost;

	CSoundPatch *m_pPilotLightSound;

	bool m_bOldHitTarget;
	CSoundPatch *m_pHitTargetSound;
#else
	float m_flStopHitSoundTime;
#endif

	CTFFlameThrower( const CTFFlameThrower & );
};

//=============================================================================
#define	TF_FLAMETHROWER_ROCKET_DAMAGE				15
#define TF_FLAMETHROWER_ROCKET_BURN_RADIUS			198

// Generic flame rocket.
class CTFFlameRocket : public CTFProjectile_Rocket
{
	DECLARE_CLASS( CTFFlameRocket, CTFProjectile_Rocket );
public:
	DECLARE_NETWORKCLASS(); 

	virtual int		GetWeaponID( void ) const		{ return TF_WEAPON_FLAMETHROWER_ROCKET; }
	virtual float	GetRadius() { return TF_FLAMETHROWER_ROCKET_BURN_RADIUS; }

#ifdef GAME_DLL
	// Creation.
	static CTFFlameRocket *Create( const Vector &vecOrigin, const QAngle &vecAngles, CBaseEntity *pOwner = NULL );	
	virtual void	Spawn();
	virtual void	Precache();
#else
	virtual const char *GetTrailParticleName( void ) { return "incendiaryrockettrail"; }
#endif
};

#endif // TF_WEAPON_FLAMETHROWER_H
