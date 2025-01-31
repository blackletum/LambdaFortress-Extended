//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======
//
//
//=============================================================================
#ifndef TF_WEAPON_PISTOL_H
#define TF_WEAPON_PISTOL_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_gun.h"
#include "tf_weapon_shotgun.h"

// Client specific.
#ifdef CLIENT_DLL
#define CTFPistol C_TFPistol
#define CTFPistol_Scout C_TFPistol_Scout
#define CTFPistol_ScoutSecondary C_TFPistol_ScoutSecondary
#define CTFPistol_ScoutPrimary C_TFPistol_ScoutPrimary
#endif

// The faster the player fires, the more inaccurate he becomes
#define	PISTOL_ACCURACY_SHOT_PENALTY_TIME		0.2f	// Applied amount of time each shot adds to the time we must recover from
#define	PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME	1.5f	// Maximum time penalty we'll allow

//=============================================================================
//
// TF Weapon Pistol.
//
class CTFPistol : public CTFWeaponBaseGun
{
public:

	DECLARE_CLASS( CTFPistol, CTFWeaponBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	//DECLARE_ACTTABLE();

	CTFPistol() {}
	~CTFPistol() {}

	virtual void	PrimaryAttack( void );

	virtual int		GetWeaponID( void ) const			{ return TF_WEAPON_PISTOL; }

private:
	CTFPistol( const CTFPistol & ) {}
};

// Scout specific version
class CTFPistol_Scout : public CTFPistol
{
public:
	DECLARE_CLASS( CTFPistol_Scout, CTFPistol );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	virtual int		GetWeaponID( void ) const			{ return TF_WEAPON_PISTOL_SCOUT; }
};

// Live tf2 weapons
class CTFPistol_ScoutSecondary : public CTFPistol_Scout
{
public:
	DECLARE_CLASS( CTFPistol_ScoutSecondary, CTFPistol_Scout );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	virtual int		GetWeaponID( void ) const			{ return TF_WEAPON_HANDGUN_SCOUT_SECONDARY; }
};

class CTFPistol_ScoutPrimary : public CTFShotgun
{
public:
	DECLARE_CLASS( CTFPistol_ScoutPrimary, CTFShotgun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	virtual void	PrimaryAttack( void ) { m_bReloadsSingly = false; BaseClass::PrimaryAttack();}

	virtual int		GetWeaponID( void ) const			{ return TF_WEAPON_HANDGUN_SCOUT_PRIMARY; }
};

#endif // TF_WEAPON_PISTOL_H