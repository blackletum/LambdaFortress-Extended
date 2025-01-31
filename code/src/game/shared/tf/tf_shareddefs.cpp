//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "tf_shareddefs.h"
#include "KeyValues.h"
#include "takedamageinfo.h"
#include "tf_gamerules.h"

//-----------------------------------------------------------------------------
// Teams.
//-----------------------------------------------------------------------------
const char *g_aTeamNames[TF_TEAM_COUNT] =
{
	"Unassigned",
	"Spectator",
	"Red",
	"Blue",
	"Green",
	"Yellow"
};

const char *g_aTeamNamesShort[TF_TEAM_COUNT] =
{
	"red", // Unassigned
	"red", // Spectator
	"red",
	"blu",
	"grn",
	"ylw"
};

const char *g_aTeamParticleNames[TF_TEAM_COUNT] =
{
	"red",
	"red",
	"red",
	"blue",
	"green",
	"yellow"
};

const char *GetTeamParticleName( int iTeam, bool bHasFourTeam /*= false*/, const char **pNames/* = g_aTeamParticleNames*/ )
{
	return pNames[iTeam];
}

const char *ConstructTeamParticle( const char *pszFormat, int iTeam, bool bHasFourTeam /*= false*/, const char **pNames/* = g_aTeamParticleNames*/ )
{
	static char szParticleName[128];

	V_snprintf( szParticleName, sizeof( szParticleName ), pszFormat, GetTeamParticleName( iTeam, bHasFourTeam, pNames ) );
	return szParticleName;
}

void PrecacheTeamParticles( const char *pszFormat, bool bHasFourTeam /*= false*/, const char **pNames/* = g_aTeamParticleNames*/ )
{
	for ( int i = FIRST_GAME_TEAM; i < TF_TEAM_GREEN; i++ )
	{
		const char *pszParticle = ConstructTeamParticle( pszFormat, i, false, pNames );
		PrecacheParticleSystem( pszParticle );
	}

	if ( bHasFourTeam )
	{
		for ( int i = FIRST_GAME_TEAM; i < TF_TEAM_COUNT; i++ )
		{
			const char *pszParticle = ConstructTeamParticle( pszFormat, i, false, pNames );
			PrecacheParticleSystem( pszParticle );
		}
	}
}

color32 g_aTeamColors[TF_TEAM_COUNT] = 
{
	{ 0, 0, 0, 0 }, // Unassigned
	{ 0, 0, 0, 0 }, // Spectator
	{ 255, 0, 0, 0 }, // Red
	{ 0, 0, 255, 0 }, // Blue
	{ 0, 255, 0, 0 }, // Green
	{ 255, 255, 0, 0 } // Yellow
};

bool IsGameTeam( int iTeam )
{
	return ( iTeam > LAST_SHARED_TEAM && iTeam < TF_TEAM_COUNT ); 
}

//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------

const char *g_aPlayerClassNames[] =
{
	"#TF_Class_Name_Undefined",
	"#TF_Class_Name_Scout",
	"#TF_Class_Name_Sniper",
	"#TF_Class_Name_Soldier",
	"#TF_Class_Name_Demoman",
	"#TF_Class_Name_Medic",
	"#TF_Class_Name_HWGuy",
	"#TF_Class_Name_Pyro",
	"#TF_Class_Name_Spy",
	"#TF_Class_Name_Engineer",
};

const char *g_aPlayerClassEmblems[] =
{
	"../hud/leaderboard_class_scout",
	"../hud/leaderboard_class_sniper",
	"../hud/leaderboard_class_soldier",
	"../hud/leaderboard_class_demo",
	"../hud/leaderboard_class_medic",
	"../hud/leaderboard_class_heavy",
	"../hud/leaderboard_class_pyro",
	"../hud/leaderboard_class_spy",
	"../hud/leaderboard_class_engineer",
};

const char *g_aPlayerClassEmblemsDead[] =
{
	"../hud/leaderboard_class_scout_d",
	"../hud/leaderboard_class_sniper_d",
	"../hud/leaderboard_class_soldier_d",
	"../hud/leaderboard_class_demo_d",
	"../hud/leaderboard_class_medic_d",
	"../hud/leaderboard_class_heavy_d",
	"../hud/leaderboard_class_pyro_d",
	"../hud/leaderboard_class_spy_d",
	"../hud/leaderboard_class_engineer_d",
};

const char *g_aPlayerClassEmblemsAlt[] =
{
	"../vgui/class_icons/class_icon_orange_scout",
	"../vgui/class_icons/class_icon_orange_sniper",
	"../vgui/class_icons/class_icon_orange_soldier",
	"../vgui/class_icons/class_icon_orange_demo",
	"../vgui/class_icons/class_icon_orange_medic",
	"../vgui/class_icons/class_icon_orange_heavy",
	"../vgui/class_icons/class_icon_orange_pyro",
	"../vgui/class_icons/class_icon_orange_spy",
	"../vgui/class_icons/class_icon_orange_engineer",
};

const char *g_aPlayerClassEmblemsAltDead[] =
{
	"../vgui/class_icons/class_icon_orange_scout_d",
	"../vgui/class_icons/class_icon_orange_sniper_d",
	"../vgui/class_icons/class_icon_orange_soldier_d",
	"../vgui/class_icons/class_icon_orange_demo_d",
	"../vgui/class_icons/class_icon_orange_medic_d",
	"../vgui/class_icons/class_icon_orange_heavy_d",
	"../vgui/class_icons/class_icon_orange_pyro_d",
	"../vgui/class_icons/class_icon_orange_spy_d",
	"../vgui/class_icons/class_icon_orange_engineer_d",
};

const char *g_aPlayerClassNames_NonLocalized[] =
{
	"Undefined",
	"Scout",
	"Sniper",
	"Soldier",
	"Demoman",
	"Medic",
	"Heavy",
	"Pyro",
	"Spy",
	"Engineer",
};

const char *g_aRawPlayerClassNamesShort[] =
{
	"undefined",
	"scout",
	"sniper",
	"soldier",
	"demo",
	"medic",
	"heavy",
	"pyro",
	"spy",
	"engineer",
	"civilian",
	"",
	"random"
};

const char *g_aRawPlayerClassNames[] =
{
	"undefined",
	"scout",
	"sniper",
	"soldier",
	"demoman",
	"medic",
	"heavyweapons",
	"pyro",
	"spy",
	"engineer",
	"civilian",
	"",
	"random"
};

//-----------------------------------------------------------------------------
// Gametypes.
//-----------------------------------------------------------------------------
const char *g_aGameTypeNames[] =
{
	"Undefined",
	"#Gametype_CTF",
	"#Gametype_CP",
	"#Gametype_Escort",
	"#Gametype_Arena",
	"#Gametype_RobotDestruction",
	"#GameType_Passtime",
	"#GameType_PlayerDestruction",
	"#Gametype_MVM",
	"#Gametype_CoOp",
	"#Gametype_VS",
	"#Gametype_BLUCoOp",
	"#Gametype_ZS",
};

//-----------------------------------------------------------------------------
// Weapon Types
//-----------------------------------------------------------------------------
const char *g_AnimSlots[] =
{
	"primary",
	"secondary",
	"melee",
	"grenade",
	"building",
	"pda",
	"item1",
	"item2",
	"MELEE_ALLCLASS",
	"secondary2",
	"primary2"
};

const char *g_LoadoutSlots[] =
{
	"primary",
	"secondary",
	"melee",
	"pda",
	"pda2",
	"building",
	"head",
	"misc",
	"action"
};

//-----------------------------------------------------------------------------
// Ammo.
//-----------------------------------------------------------------------------
const char *g_aAmmoNames[] =
{
	"DUMMY AMMO",
	"TF_AMMO_PRIMARY",
	"TF_AMMO_SECONDARY",
	"TF_AMMO_METAL",
	"TF_AMMO_GRENADES1",
	"TF_AMMO_GRENADES2"
};

struct pszWpnEntTranslationListEntry
{
	const char *weapon_name;
	const char *padding;
	const char *weapon_scout;
	const char *weapon_sniper;
	const char *weapon_soldier;
	const char *weapon_demoman;
	const char *weapon_medic;
	const char *weapon_heavyweapons;
	const char *weapon_pyro;
	const char *weapon_spy;
	const char *weapon_engineer;
};
static pszWpnEntTranslationListEntry pszWpnEntTranslationList[] =
{
	"tf_weapon_shotgun",			// Base weapon to translate
	NULL,
	"tf_weapon_shotgun_primary",	// Scout
	"tf_weapon_shotgun_primary",	// Sniper
	"tf_weapon_shotgun_soldier",	// Soldier
	"tf_weapon_shotgun_primary",	// Demoman
	"tf_weapon_shotgun_primary",	// Medic
	"tf_weapon_shotgun_hwg",		// Heavy
	"tf_weapon_shotgun_pyro",		// Pyro
	"tf_weapon_shotgun_primary",	// Spy
	"tf_weapon_shotgun_primary",	// Engineer

	"tf_weapon_pistol",				// Base weapon to translate
	NULL,
	"tf_weapon_pistol_scout",		// Scout
	"tf_weapon_pistol",				// Sniper
	"tf_weapon_pistol",				// Soldier
	"tf_weapon_pistol",				// Demoman
	"tf_weapon_pistol",				// Medic
	"tf_weapon_pistol",				// Heavy
	"tf_weapon_pistol",				// Pyro
	"tf_weapon_pistol",				// Spy
	"tf_weapon_pistol",				// Engineer

	"tf_weapon_shovel",				// Base weapon to translate
	NULL,
	"tf_weapon_shovel",				// Scout
	"tf_weapon_shovel",				// Sniper
	"tf_weapon_shovel",				// Soldier
	"tf_weapon_bottle",				// Demoman
	"tf_weapon_shovel",				// Medic
	"tf_weapon_shovel",				// Heavy
	"tf_weapon_shovel",				// Pyro
	"tf_weapon_shovel",				// Spy
	"tf_weapon_shovel",				// Engineer

	"tf_weapon_bottle",				// Base weapon to translate
	NULL,
	"tf_weapon_bottle",				// Scout
	"tf_weapon_bottle",				// Sniper
	"tf_weapon_shovel",				// Soldier
	"tf_weapon_bottle",				// Demoman
	"tf_weapon_bottle",				// Medic
	"tf_weapon_bottle",				// Heavy
	"tf_weapon_bottle",				// Pyro
	"tf_weapon_bottle",				// Spy
	"tf_weapon_bottle",				// Engineer

	"saxxy",						// Base weapon to translate
	NULL,
	"tf_weapon_bat",				// Scout
	"tf_weapon_club",				// Sniper
	"tf_weapon_shovel",				// Soldier
	"tf_weapon_bottle",				// Demoman
	"tf_weapon_bonesaw",			// Medic
	"tf_weapon_fireaxe",			// Heavy
	"tf_weapon_fireaxe",			// Pyro
	"tf_weapon_knife",				// Spy
	"tf_weapon_wrench",				// Engineer

	"tf_weapon_throwable",			// Base weapon to translate
	NULL,
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88D0
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88B2
	"tf_weapon_throwable", //UNK_10D88B2

	"tf_weapon_parachute",			// Base weapon to translate
	NULL,
	"tf_weapon_parachute_secondary",	// Scout
	"tf_weapon_parachute_secondary",	// Sniper
	"tf_weapon_parachute_primary",		// Soldier
	"tf_weapon_parachute_secondary",	// Demoman
	"tf_weapon_parachute_secondary",	// Medic
	"tf_weapon_parachute_secondary",	// Heavy
	"tf_weapon_parachute_secondary",	// Pyro
	"tf_weapon_parachute_secondary",	// Spy
	0,									// Engineer

	"tf_weapon_revolver",			// Base weapon to translate
	NULL,
	"tf_weapon_revolver",			// Scout
	"tf_weapon_revolver",			// Sniper
	"tf_weapon_revolver",			// Soldier
	"tf_weapon_revolver",			// Demoman
	"tf_weapon_revolver",			// Medic
	"tf_weapon_revolver",			// Heavy
	"tf_weapon_revolver",			// Pyro
	"tf_weapon_revolver",			// Spy
	"tf_weapon_revolver_secondary",	// Engineer

	"tf_weapon_physcannon",			// Base weapon to translate
	NULL,
	"tf_weapon_physcannon",			// Scout
	"tf_weapon_physcannon",			// Sniper
	"tf_weapon_physcannon",			// Soldier
	"tf_weapon_physcannon",			// Demoman
	"tf_weapon_physcannon_secondary",	// Medic
	"tf_weapon_physcannon",			// Heavy
	"tf_weapon_physcannon",			// Pyro
	"tf_weapon_physcannon_secondary",	// Spy
	"tf_weapon_physcannon",	// Engineer
/*
	"tf_weapon_physgun",			// Base weapon to translate
	NULL,
	"tf_weapon_physgun",			// Scout
	"tf_weapon_physgun",			// Sniper
	"tf_weapon_physgun",			// Soldier
	"tf_weapon_physgun",			// Demoman
	"tf_weapon_physgun",			// Medic
	"tf_weapon_physgun",			// Heavy
	"tf_weapon_physgun",			// Pyro
	"tf_weapon_physgun",			// Spy
	"tf_weapon_physgun_secondary",	// Engineer*/
};

//-----------------------------------------------------------------------------
// Weapons.
//-----------------------------------------------------------------------------
const char *g_aWeaponNames[] =
{
	"TF_WEAPON_NONE",
	"TF_WEAPON_BAT",
	"TF_WEAPON_BOTTLE", 
	"TF_WEAPON_FIREAXE",
	"TF_WEAPON_CLUB",
	"TF_WEAPON_CROWBAR",
	"TF_WEAPON_KNIFE",
	"TF_WEAPON_FISTS",
	"TF_WEAPON_SHOVEL",
	"TF_WEAPON_WRENCH",
	"TF_WEAPON_BONESAW",
	"TF_WEAPON_SHOTGUN_PRIMARY",
	"TF_WEAPON_SHOTGUN_SOLDIER",
	"TF_WEAPON_SHOTGUN_HWG",
	"TF_WEAPON_SHOTGUN_PYRO",
	"TF_WEAPON_SCATTERGUN",
	"TF_WEAPON_SNIPERRIFLE",
	"TF_WEAPON_MINIGUN",
	"TF_WEAPON_SMG",
	"TF_WEAPON_SYRINGEGUN_MEDIC",
	"TF_WEAPON_TRANQ",
	"TF_WEAPON_ROCKETLAUNCHER",
	"TF_WEAPON_GRENADELAUNCHER",
	"TF_WEAPON_PIPEBOMBLAUNCHER",
	"TF_WEAPON_FLAMETHROWER",
	"TF_WEAPON_GRENADE_NORMAL",
	"TF_WEAPON_GRENADE_CONCUSSION",
	"TF_WEAPON_GRENADE_NAIL",
	"TF_WEAPON_GRENADE_MIRV",
	"TF_WEAPON_GRENADE_MIRV_DEMOMAN",
	"TF_WEAPON_GRENADE_NAPALM",
	"TF_WEAPON_GRENADE_GAS",
	"TF_WEAPON_GRENADE_EMP",
	"TF_WEAPON_GRENADE_CALTROP",
	"TF_WEAPON_GRENADE_PIPEBOMB",
	"TF_WEAPON_GRENADE_SMOKE_BOMB",
	"TF_WEAPON_GRENADE_HEAL",
	"TF_WEAPON_PISTOL",
	"TF_WEAPON_PISTOL_SCOUT",
	"TF_WEAPON_REVOLVER",
	"TF_WEAPON_NAILGUN",
	"TF_WEAPON_PDA",
	"TF_WEAPON_PDA_ENGINEER_BUILD",
	"TF_WEAPON_PDA_ENGINEER_DESTROY",
	"TF_WEAPON_PDA_SPY",
	"TF_WEAPON_BUILDER",
	"TF_WEAPON_MEDIGUN",
	"TF_WEAPON_GRENADE_MIRVBOMB",
	"TF_WEAPON_FLAMETHROWER_ROCKET",
	"TF_WEAPON_GRENADE_DEMOMAN",
	"TF_WEAPON_SENTRY_BULLET",
	"TF_WEAPON_SENTRY_ROCKET",
	"TF_WEAPON_DISPENSER",
	"TF_WEAPON_INVIS",
	"TF_WEAPON_FLAG",
	"TF_WEAPON_FLAREGUN",
	"TF_WEAPON_LUNCHBOX",
	"TF_WEAPON_LUNCHBOX_DRINK",
	"TF_WEAPON_COMPOUND_BOW",

	"TF_WEAPON_BAT_FISH",
	"TF_WEAPON_BAT_GIFTWARP",
	"TF_WEAPON_BAT_WOOD",
	"TF_WEAPON_BREAKABLE_SIGN",
	"TF_WEAPON_BUFF_ITEM",
	"TF_WEAPON_CANNON",
	"TF_WEAPON_CHARGED_SMG",
	"TF_WEAPON_CLEAVER",
	"TF_WEAPON_CROSSBOW",
	"TF_WEAPON_DRG_POMSON",
	"TF_WEAPON_FLAREGUN_REVENGE",
	"TF_WEAPON_GRAPPLINGHOOK",
	"TF_WEAPON_HANDGUN_SCOUT_PRIMARY",
	"TF_WEAPON_HANDGUN_SCOUT_SECONDARY",
	"TF_WEAPON_JAR",
	"TF_WEAPON_JAR_GAS",
	"TF_WEAPON_JAR_MILK",
	"TF_WEAPON_KANATA",
	"TF_WEAPON_LASER_POINTER",
	"TF_WEAPON_MECHANICAL_ARM",
	"TF_WEAPON_PARACHUTE",
	"TF_WEAPON_PARACHUTE_PRIMARY",
	"TF_WEAPON_PARACHUTE_SECONDARY",
	"TF_WEAPON_PARTICLE_CANNON",
	"TF_WEAPON_PASSTIME_GUN",
	"TF_WEAPON_PEP_BRAWLER_BLASTER",
	"TF_WEAPON_RAYGUN",
	"TF_WEAPON_ROBOT_ARM",
	"TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE",
	"TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT",
	"TF_WEAPON_ROCKETLAUNCHER_FIREBALL", //"TF_WEPON_FLAME_BALL",
	"TF_WEAPON_ROCKETPACK",
	"TF_WEAPON_SENTRY_REVENGE",
	"TF_WEAPON_SHOTGUN_BUILDING_RESCUE",
	"TF_WEAPON_SLAP",
	"TF_WEAPON_SNIPERRIFLE_CLASSIC",
	"TF_WEAPON_SNIPERRIFLE_DECAP",
	"TF_WEAPON_SODA_POPPER",
	"TF_WEAPON_SPELLBOOK",
	"TF_WEAPON_SWORD",

	"TF_WEAPON_STICKBOMB",
	// ADD NEW WEAPONS AFTER THIS

	"TF_WEAPON_PHYSCANNON",
	//"TF_WEAPON_PHYSGUN",

	"TF_WEAPON_COUNT",	// end marker, do not add below here
};
#pragma warning( push )
#pragma warning( disable : 4838 )
int g_aWeaponDamageTypes[] =
{
	DMG_GENERIC,	// TF_WEAPON_NONE
	DMG_CLUB,		// TF_WEAPON_BAT,
	DMG_CLUB,		// TF_WEAPON_BOTTLE, 
	DMG_CLUB,		// TF_WEAPON_FIREAXE,
	DMG_CLUB,		// TF_WEAPON_CLUB,
	DMG_CLUB,		// TF_WEAPON_CROWBAR,
	DMG_SLASH,		// TF_WEAPON_KNIFE,
	DMG_CLUB,		// TF_WEAPON_FISTS,
	DMG_CLUB,		// TF_WEAPON_SHOVEL,
	DMG_CLUB,		// TF_WEAPON_WRENCH,
	DMG_SLASH,		// TF_WEAPON_BONESAW,
	DMG_BULLET | DMG_BUCKSHOT | DMG_USEDISTANCEMOD,	// TF_WEAPON_SHOTGUN_PRIMARY,
	DMG_BULLET | DMG_BUCKSHOT | DMG_USEDISTANCEMOD,	// TF_WEAPON_SHOTGUN_SOLDIER,
	DMG_BULLET | DMG_BUCKSHOT | DMG_USEDISTANCEMOD,	// TF_WEAPON_SHOTGUN_HWG,
	DMG_BULLET | DMG_BUCKSHOT | DMG_USEDISTANCEMOD,	// TF_WEAPON_SHOTGUN_PYRO,
	DMG_BULLET | DMG_BUCKSHOT | DMG_USEDISTANCEMOD,  // TF_WEAPON_SCATTERGUN,
	DMG_BULLET | DMG_USE_HITLOCATIONS,	// TF_WEAPON_SNIPERRIFLE,
	DMG_BULLET | DMG_USEDISTANCEMOD,		// TF_WEAPON_MINIGUN,
	DMG_BULLET | DMG_USEDISTANCEMOD,		// TF_WEAPON_SMG,
	DMG_BULLET | DMG_USEDISTANCEMOD | DMG_NOCLOSEDISTANCEMOD | DMG_PREVENT_PHYSICS_FORCE,		// TF_WEAPON_SYRINGEGUN_MEDIC,
	DMG_BULLET | DMG_USEDISTANCEMOD | DMG_PREVENT_PHYSICS_FORCE | DMG_PARALYZE,		// TF_WEAPON_TRANQ,
	DMG_BLAST | DMG_HALF_FALLOFF | DMG_USEDISTANCEMOD,		// TF_WEAPON_ROCKETLAUNCHER,
	DMG_BLAST | DMG_HALF_FALLOFF | DMG_USEDISTANCEMOD,		// TF_WEAPON_GRENADELAUNCHER,
	DMG_BLAST | DMG_HALF_FALLOFF,		// TF_WEAPON_PIPEBOMBLAUNCHER,
	DMG_IGNITE | DMG_PREVENT_PHYSICS_FORCE | DMG_PREVENT_PHYSICS_FORCE,		// TF_WEAPON_FLAMETHROWER,
	DMG_BLAST | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_NORMAL,
	DMG_SONIC | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_CONCUSSION,
	DMG_BULLET | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_NAIL,
	DMG_BLAST | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_MIRV,
	DMG_BLAST | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_MIRV_DEMOMAN,
	DMG_BURN | DMG_RADIUS_MAX,		// TF_WEAPON_GRENADE_NAPALM,
	DMG_POISON | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_GAS,
	DMG_BLAST | DMG_HALF_FALLOFF | DMG_PREVENT_PHYSICS_FORCE,		// TF_WEAPON_GRENADE_EMP,
	DMG_GENERIC,	// TF_WEAPON_GRENADE_CALTROP,
	DMG_BLAST | DMG_HALF_FALLOFF,		// TF_WEAPON_GRENADE_PIPEBOMB,
	DMG_GENERIC,	// TF_WEAPON_GRENADE_SMOKE_BOMB,
	DMG_GENERIC,	// TF_WEAPON_GRENADE_HEAL
	DMG_BULLET | DMG_USEDISTANCEMOD,		// TF_WEAPON_PISTOL,
	DMG_BULLET | DMG_USEDISTANCEMOD,		// TF_WEAPON_PISTOL_SCOUT,
	DMG_BULLET | DMG_USEDISTANCEMOD,		// TF_WEAPON_REVOLVER,
	DMG_BULLET | DMG_USEDISTANCEMOD | DMG_NOCLOSEDISTANCEMOD | DMG_PREVENT_PHYSICS_FORCE,		// TF_WEAPON_NAILGUN,
	DMG_BULLET,		// TF_WEAPON_PDA,
	DMG_BULLET,		// TF_WEAPON_PDA_ENGINEER_BUILD,
	DMG_BULLET,		// TF_WEAPON_PDA_ENGINEER_DESTROY,
	DMG_BULLET,		// TF_WEAPON_PDA_SPY,
	DMG_BULLET,		// TF_WEAPON_BUILDER
	DMG_BULLET,		// TF_WEAPON_MEDIGUN
	DMG_BLAST,		// TF_WEAPON_GRENADE_MIRVBOMB
	DMG_BLAST | DMG_IGNITE | DMG_RADIUS_MAX,		// TF_WEAPON_FLAMETHROWER_ROCKET
	DMG_BLAST | DMG_HALF_FALLOFF,					// TF_WEAPON_GRENADE_DEMOMAN
	DMG_GENERIC,	// TF_WEAPON_SENTRY_BULLET
	DMG_GENERIC,	// TF_WEAPON_SENTRY_ROCKET
	DMG_GENERIC,	// TF_WEAPON_DISPENSER
	DMG_GENERIC,	// TF_WEAPON_INVIS
	DMG_GENERIC,	// TF_WEAPON_FLAG
	DMG_IGNITE,		// TF_WEAPON_FLAREGUN,
	DMG_GENERIC,	// TF_WEAPON_LUNCHBOX,
	DMG_GENERIC,	// TF_WEAPON_LUNCHBOX_DRINK,
	DMG_BULLET,		// TF_WEAPON_COMPOUND_BOW

	DMG_CLUB,		// TF_WEAPON_BAT_FISH,
	DMG_CLUB,		// TF_WEAPON_BAT_GIFTWARP,
	DMG_CLUB,		// TF_WEAPON_BAT_WOOD,
	DMG_CLUB,		// TF_WEAPON_BREAKABLE_SIGN,
	DMG_GENERIC,	// TF_WEAPON_BUFF_ITEM,
	DMG_BLAST | DMG_HALF_FALLOFF,	// TF_WEAPON_CANNON,
	DMG_BULLET,	// TF_WEAPON_CHARGED_SMG,
	DMG_SLASH,	// TF_WEAPON_CLEAVER,
	DMG_BULLET,	// TF_WEAPON_CROSSBOW,
	DMG_BULLET,	// TF_WEAPON_DRG_POMSON,
	DMG_IGNITE,	// TF_WEAPON_FLAREGUN_REVENGE,
	DMG_BULLET,	// TF_WEAPON_GRAPPLINGHOOK,
	DMG_BULLET,	// TF_WEAPON_HANDGUN_SCOUT_PRIMARY,
	DMG_BULLET,	// TF_WEAPON_HANDGUN_SCOUT_SECONDARY,
	DMG_GENERIC | DMG_USEDISTANCEMOD,	// TF_WEAPON_JAR,
	DMG_GENERIC | DMG_USEDISTANCEMOD,	// TF_WEAPON_JAR_GAS,
	DMG_GENERIC | DMG_USEDISTANCEMOD,	// TF_WEAPON_JAR_MILK,
	DMG_CLUB,	// TF_WEAPON_KATANA,
	DMG_GENERIC,	// TF_WEAPON_LASER_POINTER,
	DMG_CLUB,	// TF_WEAPON_MECHANICAL_ARM,
	DMG_GENERIC,	// TF_WEAPON_PARACHUTE,
	DMG_GENERIC,	// TF_WEAPON_PARACHUTE_PRIMARY,
	DMG_GENERIC,	// TF_WEAPON_PARACHUTE_SECONDARY,
	DMG_BLAST | DMG_HALF_FALLOFF | DMG_USEDISTANCEMOD,	// TF_WEAPON_PARTICLE_CANNON,
	DMG_GENERIC,	// TF_WEAPON_PASSTIME_GUN,
	DMG_GENERIC,	// TF_WEAPON_PEP_BRAWLER_BLASTER,
	DMG_BULLET,	// TF_WEAPON_RAYGUN,
	DMG_CLUB,	// TF_WEAPON_ROBOT_ARM,
	DMG_BLAST | DMG_HALF_FALLOFF | DMG_USEDISTANCEMOD,	// TF_WEAPON_ROCKETLAUNCHER_AIRSTRIKE,
	DMG_BLAST | DMG_HALF_FALLOFF | DMG_USEDISTANCEMOD,	// TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT,
	DMG_IGNITE | DMG_USEDISTANCEMOD,	// TF_WEAPON_ROCKETLAUNCHER_FIREBALL //TF_WEPON_FLAME_BALL,
	DMG_GENERIC,	// TF_WEAPON_ROCKETPACK,
	DMG_BULLET,	// TF_WEAPON_SENTRY_REVENGE,
	DMG_BULLET,	// TF_WEAPON_SHOTGUN_BUILDING_RESCUE,
	DMG_CLUB,	// TF_WEAPON_SLAP,
	DMG_BULLET | DMG_USE_HITLOCATIONS,	// TF_WEAPON_SNIPERRIFLE_CLASSIC,
	DMG_BULLET | DMG_USE_HITLOCATIONS,	// TF_WEAPON_SNIPERRIFLE_DECAP,
	DMG_BULLET,	// TF_WEAPON_SODA_POPPER,
	DMG_GENERIC,	// TF_WEAPON_SPELLBOOK,
	DMG_CLUB,	// TF_WEAPON_SWORD,

	DMG_CLUB,	// TF_WEAPON_STICKBOMB,

	// ADD NEW WEAPONS AFTER THIS
	DMG_GENERIC,	// TF_WEAPON_PHYSCANNON
	//DMG_GENERIC,	// TF_WEAPON_PHYSGUN

	// This is a special entry that must match with TF_WEAPON_COUNT
	// to protect against updating the weapon list without updating this list
	TF_DMG_SENTINEL_VALUE
};

#pragma warning(pop)
// Spread pattern for tf_use_fixed_weaponspreads.
const Vector g_vecFixedWpnSpreadPellets[] =
{
	Vector( 0, 0, 0 ),
	Vector( 1, 0, 0 ),
	Vector( -1, 0, 0 ),
	Vector( 0, -1, 0 ),
	Vector( 0, 1, 0 ),
	Vector( 0.85, -0.85, 0 ),
	Vector( 0.85, 0.85, 0 ),
	Vector( -0.85, -0.85, 0 ),
	Vector( -0.85, 0.85, 0 ),
	Vector( 0, 0, 0 ),
};

const char *g_szProjectileNames[] =
{
	"",
	"projectile_bullet",
	"projectile_rocket",
	"projectile_pipe",
	"projectile_pipe_remote",
	"projectile_syringe",
	"projectile_flare",
	"projectile_jar",
	"projectile_arrow",
	"projectile_flame_rocket",
	"projectile_jar_milk",
	"projectile_healing_bolt",
	"projectile_energy_ball",
	"projectile_energy_ring",
	"projectile_pipe_remote_practice",
	"projectile_cleaver",
	"projectile_sticky_ball",
	"projectile_cannonball",
	"projectile_building_repair_bolt",
	"projectile_festive_arrow",
	"projectile_throwable",
	"projectile_spellfireball",
	"projectile_festive_urine",
	"projectile_festive_healing_bolt",
	"projectfile_breadmonster_jarate",
	"projectfile_breadmonster_madmilk",
	"projectile_grapplinghook",
	"projectile_sentry_rocket",
	"projectile_bread_monster",
	"projectile_jar_gas",
	"projectile_balloffire",
	// Add new projectiles here.
	"projectile_hl2_frag",
	"projectile_hl2_combineball",
	"projectile_hl2_ar2",
	"projectile_hl2_spit",
};

// these map to the projectiles named in g_szProjectileNames
int g_iProjectileWeapons[] = 
{
	TF_WEAPON_NONE,
	TF_WEAPON_PISTOL,
	TF_WEAPON_ROCKETLAUNCHER,
	TF_WEAPON_PIPEBOMBLAUNCHER,
	TF_WEAPON_GRENADELAUNCHER,
	TF_WEAPON_SYRINGEGUN_MEDIC,
	TF_WEAPON_NAILGUN,
	TF_WEAPON_TRANQ,
	TF_WEAPON_FLAREGUN,
	TF_WEAPON_JAR,
	TF_WEAPON_COMPOUND_BOW,
	TF_WEAPON_FLAMETHROWER,
	TF_WEAPON_JAR_MILK,
	TF_WEAPON_CROSSBOW,
	TF_WEAPON_DRG_POMSON,
	TF_WEAPON_DRG_POMSON,
	TF_WEAPON_GRENADELAUNCHER,
	TF_WEAPON_CLEAVER,
	TF_WEAPON_GRENADELAUNCHER,
	TF_WEAPON_PIPEBOMBLAUNCHER,
	TF_WEAPON_SHOTGUN_BUILDING_RESCUE,
	TF_WEAPON_COMPOUND_BOW,
	TF_WEAPON_JAR,
	TF_WEAPON_SPELLBOOK,
	TF_WEAPON_JAR,
	TF_WEAPON_COMPOUND_BOW,
	TF_WEAPON_JAR,
	TF_WEAPON_JAR_MILK,
	TF_WEAPON_GRAPPLINGHOOK,
	TF_WEAPON_SENTRY_ROCKET,
	TF_WEAPON_JAR,
	TF_WEAPON_JAR_GAS,
	TF_WEAPON_ROCKETLAUNCHER_FIREBALL,
	// random stuff
	TF_WEAPON_NONE,
	TF_WEAPON_NONE,
	TF_WEAPON_NONE,
	TF_WEAPON_NONE,
};

const char *g_pszHintMessages[] =
{
	"#Hint_spotted_a_friend",
	"#Hint_spotted_an_enemy",
	"#Hint_killing_enemies_is_good",
	"#Hint_out_of_ammo",
	"#Hint_turn_off_hints",
	"#Hint_pickup_ammo",
	"#Hint_Cannot_Teleport_With_Flag",
	"#Hint_Cannot_Cloak_With_Flag",
	"#Hint_Cannot_Disguise_With_Flag",
	"#Hint_Cannot_Attack_While_Cloaked",
	"#Hint_ClassMenu",

// Grenades
	"#Hint_gren_caltrops",
	"#Hint_gren_concussion",
	"#Hint_gren_emp",
	"#Hint_gren_gas",
	"#Hint_gren_mirv",
	"#Hint_gren_nail",
	"#Hint_gren_napalm",
	"#Hint_gren_normal",

// Altfires
	"#Hint_altfire_sniperrifle",
	"#Hint_altfire_flamethrower",
	"#Hint_altfire_grenadelauncher",
	"#Hint_altfire_pipebomblauncher",
	"#Hint_altfire_rotate_building",

// Soldier
	"#Hint_Soldier_rpg_reload",

// Engineer
	"#Hint_Engineer_use_wrench_onown",
	"#Hint_Engineer_use_wrench_onother",
	"#Hint_Engineer_use_wrench_onfriend",
	"#Hint_Engineer_build_sentrygun",
	"#Hint_Engineer_build_dispenser",
	"#Hint_Engineer_build_teleporters",
	"#Hint_Engineer_pickup_metal",
	"#Hint_Engineer_repair_object",
	"#Hint_Engineer_metal_to_upgrade",
	"#Hint_Engineer_upgrade_sentrygun",

	"#Hint_object_has_sapper",

	"#Hint_object_your_object_sapped",
	"#Hint_enemy_using_dispenser",
	"#Hint_enemy_using_tp_entrance",
	"#Hint_enemy_using_tp_exit",
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int GetWeaponId( const char *pszWeaponName )
{
	// if this doesn't match, you need to add missing weapons to the array
	assert( ARRAYSIZE( g_aWeaponNames ) == ( TF_WEAPON_COUNT + 1 ) );

	for ( int iWeapon = 0; iWeapon < ARRAYSIZE( g_aWeaponNames ); ++iWeapon )
	{
		if ( !Q_stricmp( pszWeaponName, g_aWeaponNames[iWeapon] ) )
			return iWeapon;
	}

	return TF_WEAPON_NONE;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
const char *WeaponIdToAlias( int iWeapon )
{
	// if this doesn't match, you need to add missing weapons to the array
	assert( ARRAYSIZE( g_aWeaponNames ) == ( TF_WEAPON_COUNT + 1 ) );

	if ( ( iWeapon >= ARRAYSIZE( g_aWeaponNames ) ) || ( iWeapon < 0 ) )
		return NULL;

	return g_aWeaponNames[iWeapon];
}

//-----------------------------------------------------------------------------
// Purpose: Entity classnames need to be in lower case. Use this whenever
// you're spawning a weapon.
//-----------------------------------------------------------------------------
const char *WeaponIdToClassname( int iWeapon )
{
	const char *pszWeaponAlias = WeaponIdToAlias( iWeapon );

	if ( pszWeaponAlias == NULL )
		return NULL;

	static char szEntName[256];
	V_strcpy( szEntName, pszWeaponAlias );
	V_strlower( szEntName );

	return szEntName;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
const char *TranslateWeaponEntForClass( const char *pszName, int iClass )
{
	if ( pszName )
	{
		for ( int i = 0; i < ARRAYSIZE( pszWpnEntTranslationList ); i++ )
		{
			if ( V_stricmp( pszName, pszWpnEntTranslationList[i].weapon_name ) == 0 )
			{
				return ( (const char **)&( pszWpnEntTranslationList[i] ) )[1 + iClass];
			}
		}
	}
	return pszName;
}

#ifdef GAME_DLL

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int GetWeaponFromDamage( const CTakeDamageInfo &info )
{
	int iWeapon = TF_WEAPON_NONE;

	// Work out what killed the player, and send a message to all clients about it
	TFGameRules()->GetKillingWeaponName( info, NULL, iWeapon );

	return iWeapon;
}

#endif

//-----------------------------------------------------------------------------
// Conditions stuff.
//-----------------------------------------------------------------------------
int condition_to_attribute_translation[] =
{
	TF_COND_BURNING,
	TF_COND_AIMING,
	TF_COND_ZOOMED,
	TF_COND_DISGUISING,
	TF_COND_DISGUISED,
	TF_COND_STEALTHED,
	TF_COND_INVULNERABLE,
	TF_COND_TELEPORTED,
	TF_COND_TAUNTING,
	TF_COND_INVULNERABLE_WEARINGOFF,
	TF_COND_STEALTHED_BLINK,
	TF_COND_SELECTED_TO_TELEPORT,
	TF_COND_CRITBOOSTED,
	TF_COND_TMPDAMAGEBONUS,
	TF_COND_FEIGN_DEATH,
	TF_COND_PHASE,
	TF_COND_STUNNED,
	TF_COND_HEALTH_BUFF,
	TF_COND_HEALTH_OVERHEALED,
	TF_COND_URINE,
	TF_COND_ENERGY_BUFF,
	TF_COND_LAST
};

int g_aPowerupConds[] =
{
	TF_COND_RUNE_STRENGTH,
	TF_COND_RUNE_HASTE,
	TF_COND_RUNE_REGEN,
	TF_COND_RUNE_RESIST,
	TF_COND_RUNE_VAMPIRE,
	TF_COND_RUNE_WARLOCK,
	TF_COND_RUNE_PRECISION,
	TF_COND_RUNE_AGILITY,
	TF_COND_RUNE_KNOCKOUT,
	TF_COND_RUNE_KING,
	TF_COND_RUNE_PLAGUE,
	TF_COND_RUNE_SUPERNOVA,
	TF_COND_LAST
};

bool ConditionExpiresFast( int nCond )
{
	// Damaging conds
	if ( nCond == TF_COND_BURNING ||
		nCond == TF_COND_BLEEDING )
		return true;

	// Liquids
	if ( nCond == TF_COND_URINE ||
		nCond == TF_COND_MAD_MILK )
		return true;

	// Tranq
	if ( nCond == TF_COND_SLOWED )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Mediguns.
//-----------------------------------------------------------------------------
MedigunEffects_t g_MedigunEffects[] =
{
	{ TF_COND_INVULNERABLE, TF_COND_INVULNERABLE_WEARINGOFF, "TFPlayer.InvulnerableOn", "TFPlayer.InvulnerableOff" },
	{ TF_COND_CRITBOOSTED, TF_COND_LAST, "TFPlayer.CritBoostOn", "TFPlayer.CritBoostOff" },
	{ TF_COND_MEGAHEAL, TF_COND_LAST, "TFPlayer.QuickFixInvulnerableOn", "TFPlayer.MegaHealOff" },
	{ TF_COND_MEDIGUN_UBER_BULLET_RESIST, TF_COND_LAST, "WeaponMedigun_Vaccinator.InvulnerableOn", "WeaponMedigun_Vaccinator.InvulnerableOff" },
	{ TF_COND_MEDIGUN_UBER_BLAST_RESIST, TF_COND_LAST, "WeaponMedigun_Vaccinator.InvulnerableOn", "WeaponMedigun_Vaccinator.InvulnerableOff" },
	{ TF_COND_MEDIGUN_UBER_FIRE_RESIST, TF_COND_LAST, "WeaponMedigun_Vaccinator.InvulnerableOn", "WeaponMedigun_Vaccinator.InvulnerableOff" },
};

// ------------------------------------------------------------------------------------------------ //
// CObjectInfo tables.
// ------------------------------------------------------------------------------------------------ //

CObjectInfo::CObjectInfo( char *pObjectName )
{
	m_pObjectName = pObjectName;
	m_pClassName = NULL;
	m_flBuildTime = -9999;
	m_nMaxObjects = -9999;
	m_Cost = -9999;
	m_CostMultiplierPerInstance = -999;
	m_UpgradeCost = -9999;
	m_flUpgradeDuration = -9999;
	m_MaxUpgradeLevel = -9999;
	m_pBuilderWeaponName = NULL;
	m_pBuilderPlacementString = NULL;
	m_SelectionSlot = -9999;
	m_SelectionPosition = -9999;
	m_bSolidToPlayerMovement = false;
	m_pIconActive = NULL;
	m_pIconInactive = NULL;
	m_pIconMenu = NULL;
	m_pViewModel = NULL;
	m_pPlayerModel = NULL;
	m_iDisplayPriority = 0;
	m_bVisibleInWeaponSelection = true;
	m_pExplodeSound = NULL;
	m_pExplosionParticleEffect = NULL;
	m_bAutoSwitchTo = false;
	m_pUpgradeSound = NULL;
}


CObjectInfo::~CObjectInfo()
{
	delete [] m_pClassName;
	delete [] m_pStatusName;
	delete [] m_pBuilderWeaponName;
	delete [] m_pBuilderPlacementString;
	delete [] m_pIconActive;
	delete [] m_pIconInactive;
	delete [] m_pIconMenu;
	delete [] m_pViewModel;
	delete [] m_pPlayerModel;
	delete [] m_pExplodeSound;
	delete [] m_pExplosionParticleEffect;
	delete [] m_pUpgradeSound;
}

CObjectInfo g_ObjectInfos[OBJ_LAST] =
{
	CObjectInfo( "OBJ_DISPENSER" ),
	CObjectInfo( "OBJ_TELEPORTER" ),
	CObjectInfo( "OBJ_SENTRYGUN" ),
	CObjectInfo( "OBJ_ATTACHMENT_SAPPER" ),
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int GetBuildableId( const char *pszBuildableName )
{
	for ( int iBuildable = 0; iBuildable < OBJ_LAST; ++iBuildable )
	{
		if ( !Q_stricmp( pszBuildableName, g_ObjectInfos[iBuildable].m_pObjectName ) )
			return iBuildable;
	}

	return OBJ_LAST;
}

bool AreObjectInfosLoaded()
{
	return g_ObjectInfos[0].m_pClassName != NULL;
}


void LoadObjectInfos( IBaseFileSystem *pFileSystem )
{
	const char *pFilename = "scripts/objects.txt";

	// Make sure this stuff hasn't already been loaded.
	Assert( !AreObjectInfosLoaded() );

	KeyValues *pValues = new KeyValues( "Object descriptions" );
	if ( !pValues->LoadFromFile( pFileSystem, pFilename, "GAME" ) )
	{
		Error( "Can't open %s for object info.", pFilename );
		pValues->deleteThis();
		return;
	}

	// Now read each class's information in.
	for ( int iObj=0; iObj < ARRAYSIZE( g_ObjectInfos ); iObj++ )
	{
		CObjectInfo *pInfo = &g_ObjectInfos[iObj];
		KeyValues *pSub = pValues->FindKey( pInfo->m_pObjectName );
		if ( !pSub )
		{
			Error( "Missing section '%s' from %s.", pInfo->m_pObjectName, pFilename );
			pValues->deleteThis();
			return;
		}

		// Read all the info in.
		if ( (pInfo->m_flBuildTime = pSub->GetFloat( "BuildTime", -999 )) == -999 ||
			(pInfo->m_nMaxObjects = pSub->GetInt( "MaxObjects", -999 )) == -999 ||
			(pInfo->m_Cost = pSub->GetInt( "Cost", -999 )) == -999 ||
			(pInfo->m_CostMultiplierPerInstance = pSub->GetFloat( "CostMultiplier", -999 )) == -999 ||
			(pInfo->m_UpgradeCost = pSub->GetInt( "UpgradeCost", -999 )) == -999 ||
			(pInfo->m_flUpgradeDuration = pSub->GetFloat( "UpgradeDuration", -999)) == -999 ||
			(pInfo->m_MaxUpgradeLevel = pSub->GetInt( "MaxUpgradeLevel", -999 )) == -999 ||
			(pInfo->m_SelectionSlot = pSub->GetInt( "SelectionSlot", -999 )) == -999 ||
			(pInfo->m_BuildCount = pSub->GetInt( "BuildCount", -999 )) == -999 ||
			(pInfo->m_SelectionPosition = pSub->GetInt( "SelectionPosition", -999 )) == -999 )
		{
			Error( "Missing data for object '%s' in %s.", pInfo->m_pObjectName, pFilename );
			pValues->deleteThis();
			return;
		}

		pInfo->m_pClassName = ReadAndAllocStringValue( pSub, "ClassName", pFilename );
		pInfo->m_pStatusName = ReadAndAllocStringValue( pSub, "StatusName", pFilename );
		pInfo->m_pBuilderWeaponName = ReadAndAllocStringValue( pSub, "BuilderWeaponName", pFilename );
		pInfo->m_pBuilderPlacementString = ReadAndAllocStringValue( pSub, "BuilderPlacementString", pFilename );
		pInfo->m_bSolidToPlayerMovement = pSub->GetInt( "SolidToPlayerMovement", 0 ) ? true : false;
		pInfo->m_pIconActive = ReadAndAllocStringValue( pSub, "IconActive", pFilename );
		pInfo->m_pIconInactive = ReadAndAllocStringValue( pSub, "IconInactive", pFilename );
		pInfo->m_pIconMenu = ReadAndAllocStringValue( pSub, "IconMenu", pFilename );
		pInfo->m_bUseItemInfo = pSub->GetInt( "UseItemInfo", 0 ) ? true : false;
		pInfo->m_pViewModel = ReadAndAllocStringValue( pSub, "Viewmodel", pFilename );
		pInfo->m_pPlayerModel = ReadAndAllocStringValue( pSub, "Playermodel", pFilename );
		pInfo->m_iDisplayPriority = pSub->GetInt( "DisplayPriority", 0 );
		pInfo->m_pHudStatusIcon = ReadAndAllocStringValue( pSub, "HudStatusIcon", pFilename );
		pInfo->m_bVisibleInWeaponSelection = ( pSub->GetInt( "VisibleInWeaponSelection", 1 ) > 0 );
		pInfo->m_pExplodeSound = ReadAndAllocStringValue( pSub, "ExplodeSound", pFilename );
		pInfo->m_pUpgradeSound = ReadAndAllocStringValue( pSub, "UpgradeSound", pFilename );
		pInfo->m_pExplosionParticleEffect = ReadAndAllocStringValue( pSub, "ExplodeEffect", pFilename );
		pInfo->m_bAutoSwitchTo = ( pSub->GetInt( "autoswitchto", 0 ) > 0 );

		pInfo->m_iMetalToDropInGibs = pSub->GetInt( "MetalToDropInGibs", 0 );
		pInfo->m_bRequiresOwnBuilder = pSub->GetBool( "RequiresOwnBuilder", 0 );
		// PistonMiner: Added Object Mode key
		KeyValues *pAltModes = pSub->FindKey("AltModes");
		if (pAltModes)
		{
			for (int i = 0; i < 4; ++i) // load at most 4 object modes
			{
				char altModeBuffer[256]; // Max size of 0x100
				V_snprintf(altModeBuffer, ARRAYSIZE(altModeBuffer), "AltMode%d", i);
				KeyValues *pCurAltMode = pAltModes->FindKey(altModeBuffer);
				if (!pCurAltMode)
					break;

				// Save logic here
				pInfo->m_AltModes.AddToTail(ReadAndAllocStringValue( pCurAltMode, "StatusName", pFilename ));
				pInfo->m_AltModes.AddToTail(ReadAndAllocStringValue( pCurAltMode, "ModeName", pFilename ));
				pInfo->m_AltModes.AddToTail(ReadAndAllocStringValue( pCurAltMode, "IconMenu", pFilename ));
			}
		}
	}

	pValues->deleteThis();
}


const CObjectInfo* GetObjectInfo( int iObject )
{
	Assert( iObject >= 0 && iObject < OBJ_LAST );
	Assert( AreObjectInfosLoaded() );
	return &g_ObjectInfos[iObject];
}

ConVar tf_cheapobjects( "tf_cheapobjects","0", FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY | FCVAR_REPLICATED, "Set to 1 and all objects will cost 0" );

//-----------------------------------------------------------------------------
// Purpose: Return the cost of another object of the specified type
//			If bLast is set, return the cost of the last built object of the specified type
// 
// Note: Used to contain logic from tf2 that multiple instances of the same object
//       cost different amounts. See game/shared/tf_shareddefs.cpp for details
//-----------------------------------------------------------------------------
int CalculateObjectCost( int iObjectType, bool bMini /*= false*/ )
{
	if ( tf_cheapobjects.GetInt() )
	{
		return 0;
	}

	int iCost = GetObjectInfo( iObjectType )->m_Cost;

	if ( iObjectType == OBJ_SENTRYGUN && bMini )
	{
		iCost = 100;
	}

	return iCost;
}

//-----------------------------------------------------------------------------
// Purpose: Calculate the cost to upgrade an object of a specific type
//-----------------------------------------------------------------------------
int	CalculateObjectUpgrade( int iObjectType, int iObjectLevel )
{
	// Max level?
	if ( iObjectLevel >= GetObjectInfo( iObjectType )->m_MaxUpgradeLevel )
		return 0;

	int iCost = GetObjectInfo( iObjectType )->m_UpgradeCost;
	for ( int i = 0; i < (iObjectLevel - 1); i++ )
	{
		iCost *= OBJECT_UPGRADE_COST_MULTIPLIER_PER_LEVEL;
	}

	return iCost;
}

//-----------------------------------------------------------------------------
// Purpose: Return true if the specified class is allowed to build the specified object type
//-----------------------------------------------------------------------------
bool ClassCanBuild( int iClass, int iObjectType )
{
	/*
	for ( int i = 0; i < OBJ_LAST; i++ )
	{
		// Hit the end?
		if ( g_TFClassInfos[iClass].m_pClassObjects[i] == OBJ_LAST )
			return false;
		// Found it?
		if ( g_TFClassInfos[iClass].m_pClassObjects[i] == iObjectType )
			return true;
	}
	return false;
	*/

	return ( iClass == TF_CLASS_ENGINEER );
}

float g_flTeleporterRechargeTimes[] =
{
	10.0,
	5.0,
	3.0
};

float g_flDispenserAmmoRates[] =
{
	0.2,
	0.3,
	0.4
};

float g_flDispenserHealRates[] =
{
	10.0,
	15.0,
	20.0
};

const char *g_pszTFBreadModels[] =
{
	"models/weapons/c_models/c_bread/c_bread_baguette.mdl",
	"models/weapons/c_models/c_bread/c_bread_burnt.mdl",
	"models/weapons/c_models/c_bread/c_bread_cinnamon.mdl",
	"models/weapons/c_models/c_bread/c_bread_cornbread.mdl",
	"models/weapons/c_models/c_bread/c_bread_crumpet.mdl",
	"models/weapons/c_models/c_bread/c_bread_plainloaf.mdl",
	"models/weapons/c_models/c_bread/c_bread_pretzel.mdl",
	"models/weapons/c_models/c_bread/c_bread_ration.mdl",
	"models/weapons/c_models/c_bread/c_bread_russianblack.mdl",
};

const char *g_pszClassModels[TF_CLASS_COUNT_ALL] =
{
	"",
	"models/player/scout.mdl",
	"models/player/sniper.mdl",
	"models/player/soldier.mdl",
	"models/player/demo.mdl",
	"models/player/medic.mdl",
	"models/player/heavy.mdl",
	"models/player/pyro.mdl",
	"models/player/spy.mdl",
	"models/player/engineer.mdl",
};

const char *g_pszRobotClassModels[TF_CLASS_COUNT_ALL] =
{
	"",
	"models/bots/scout/bot_scout.mdl",
	"models/bots/sniper/bot_sniper.mdl",
	"models/bots/soldier/bot_soldier.mdl",
	"models/bots/demo/bot_demo.mdl",
	"models/bots/medic/bot_medic.mdl",
	"models/bots/heavy/bot_heavy.mdl",
	"models/bots/pyro/bot_pyro.mdl",
	"models/bots/spy/bot_spy.mdl",
	"models/bots/engineer/bot_engineer.mdl",
};

const char *g_pszRobotBossClassModels[TF_CLASS_COUNT_ALL] =
{
	"",
	"models/bots/scout_boss/bot_scout_boss.mdl.mdl",
	"models/bots/sniper/bot_sniper.mdl",
	"models/bots/soldier_boss/bot_soldier_boss.mdl",
	"models/bots/demo_boss/bot_demo_boss.mdl",
	"models/bots/medic/bot_medic.mdl",
	"models/bots/heavy_boss/bot_heavy_boss.mdl",
	"models/bots/pyro_boss/bot_pyro_boss.mdl",
	"models/bots/scout/bot_spy.mdl",
	"models/bots/scout/bot_engineer.mdl",
};

//-----------------------------------------------------------------------------
// ETFConds char.
//-----------------------------------------------------------------------------
const char *g_aTFCondNames[] =
{
	"TF_COND_AIMING",
	"TF_COND_ZOOMED",
	"TF_COND_DISGUISING",
	"TF_COND_DISGUISED",
	"TF_COND_STEALTHED",
	"TF_COND_INVULNERABLE",
	"TF_COND_TELEPORTED",
	"TF_COND_TAUNTING",
	"TF_COND_INVULNERABLE_WEARINGOFF",
	"TF_COND_STEALTHED_BLINK",
	"TF_COND_SELECTED_TO_TELEPORT",
	"TF_COND_CRITBOOSTED",
	"TF_COND_TMPDAMAGEBONUS",
	"TF_COND_FEIGN_DEATH",
	"TF_COND_PHASE",
	"TF_COND_STUNNED",
	"TF_COND_OFFENSEBUFF",
	"TF_COND_SHIELD_CHARGE",
	"TF_COND_DEMO_BUFF",
	"TF_COND_ENERGY_BUFF",
	"TF_COND_RADIUSHEAL",
	"TF_COND_HEALTH_BUFF",
	"TF_COND_BURNING",
	"TF_COND_HEALTH_OVERHEALED",
	"TF_COND_URINE",
	"TF_COND_BLEEDING",
	"TF_COND_DEFENSEBUFF",
	"TF_COND_MAD_MILK",
	"TF_COND_MEGAHEAL",
	"TF_COND_REGENONDAMAGEBUFF",
	"TF_COND_MARKEDFORDEATH",
	"TF_COND_NOHEALINGDAMAGEBUFF",
	"TF_COND_SPEED_BOOST",
	"TF_COND_CRITBOOSTED_PUMPKIN",
	"TF_COND_CRITBOOSTED_USER_BUFF",
	"TF_COND_CRITBOOSTED_DEMO_CHARGE",
	"TF_COND_SODAPOPPER_HYPE",
	"TF_COND_CRITBOOSTED_FIRST_BLOOD",
	"TF_COND_CRITBOOSTED_BONUS_TIME",
	"TF_COND_CRITBOOSTED_CTF_CAPTURE",
	"TF_COND_CRITBOOSTED_ON_KILL",
	"TF_COND_CANNOT_SWITCH_FROM_MELEE",
	"TF_COND_DEFENSEBUFF_NO_CRIT_BLOCK",
	"TF_COND_REPROGRAMMED",
	"TF_COND_CRITBOOSTED_RAGE_BUFF",
	"TF_COND_DEFENSEBUFF_HIGH",
	"TF_COND_SNIPERCHARGE_RAGE_BUFF",
	"TF_COND_DISGUISE_WEARINGOFF",
	"TF_COND_MARKEDFORDEATH_SILENT",
	"TF_COND_DISGUISED_AS_DISPENSER",
	"TF_COND_SAPPED",
	"TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGE",
	"TF_COND_INVULNERABLE_USER_BUFF",
	"TF_COND_HALLOWEEN_BOMB_HEAD",
	"TF_COND_HALLOWEEN_THRILLER",
	"TF_COND_RADIUSHEAL_ON_DAMAGE",
	"TF_COND_CRITBOOSTED_CARD_EFFECT",
	"TF_COND_INVULNERABLE_CARD_EFFECT",
	"TF_COND_MEDIGUN_UBER_BULLET_RESIST",
	"TF_COND_MEDIGUN_UBER_BLAST_RESIST",
	"TF_COND_MEDIGUN_UBER_FIRE_RESIST",
	"TF_COND_MEDIGUN_SMALL_BULLET_RESIST",
	"TF_COND_MEDIGUN_SMALL_BLAST_RESIST",
	"TF_COND_MEDIGUN_SMALL_FIRE_RESIST",
	"TF_COND_STEALTHED_USER_BUFF",
	"TF_COND_MEDIGUN_DEBUFF",
	"TF_COND_STEALTHED_USER_BUFF_FADING",
	"TF_COND_BULLET_IMMUNE",
	"TF_COND_BLAST_IMMUNE",
	"TF_COND_FIRE_IMMUNE",
	"TF_COND_PREVENT_DEATH",
	"TF_COND_MVM_BOT_STUN_RADIOWAVE",
	"TF_COND_HALLOWEEN_SPEED_BOOST",
	"TF_COND_HALLOWEEN_QUICK_HEAL",
	"TF_COND_HALLOWEEN_GIANT",
	"TF_COND_HALLOWEEN_TINY",
	"TF_COND_HALLOWEEN_IN_HELL",
	"TF_COND_HALLOWEEN_GHOST_MODE",
	"TF_COND_MINICRITBOOSTED_ON_KILL",
	"TF_COND_OBSCURED_SMOKE",
	"TF_COND_PARACHUTE_ACTIVE",
	"TF_COND_BLASTJUMPING",
	"TF_COND_HALLOWEEN_KART",
	"TF_COND_HALLOWEEN_KART_DASH",
	"TF_COND_BALLOON_HEAD",
	"TF_COND_MELEE_ONLY",
	"TF_COND_SWIMMING_CURSE",
	"TF_COND_FREEZE_INPUT",
	"TF_COND_HALLOWEEN_KART_CAGE",
	"TF_COND_DONOTUSE_0",
	"TF_COND_RUNE_STRENGTH",
	"TF_COND_RUNE_HASTE",
	"TF_COND_RUNE_REGEN",
	"TF_COND_RUNE_RESIST",
	"TF_COND_RUNE_VAMPIRE",
	"TF_COND_RUNE_WARLOCK",
	"TF_COND_RUNE_PRECISION",
	"TF_COND_RUNE_AGILITY",
	"TF_COND_GRAPPLINGHOOK",
	"TF_COND_GRAPPLINGHOOK_SAFEFALL",
	"TF_COND_GRAPPLINGHOOK_LATCHED",
	"TF_COND_GRAPPLINGHOOK_BLEEDING",
	"TF_COND_AFTERBURN_IMMUNE",
	"TF_COND_RUNE_KNOCKOUT",
	"TF_COND_RUNE_IMBALANCE",
	"TF_COND_CRITBOOSTED_RUNE_TEMP",
	"TF_COND_PASSTIME_INTERCEPTION",
	"TF_COND_SWIMMING_NO_EFFECTS",
	"TF_COND_PURGATORY",
	"TF_COND_RUNE_KING",
	"TF_COND_RUNE_PLAGUE",
	"TF_COND_RUNE_SUPERNOVA",
	"TF_COND_PLAGUE",
	"TF_COND_KING_BUFFED",
	"TF_COND_TEAM_GLOWS",
	"TF_COND_KNOCKED_INTO_AIR",
	"TF_COND_COMPETITIVE_WINNER",
	"TF_COND_COMPETITIVE_LOSER",
	"TF_COND_HEALING_DEBUFF",
	"TF_COND_PASSTIME_PENALTY_DEBUFF",
	"TF_COND_GRAPPLED_TO_PLAYER",
	"TF_COND_GRAPPLED_BY_PLAYER",
	"TF_COND_PARACHUTE_DEPLOYED",
	"TF_COND_GAS",
	"TF_COND_BURNING_PYRO",
	"TF_COND_ROCKETPACK",
	"TF_COND_LOST_FOOTING",
	"TF_COND_AIR_CURRENT",

	// New conds
	"TF_COND_SLOWED",
	"LFE_COND_FLASHLIGHT",
	"LFE_COND_LOW_GRAVITY",


	"TF_COND_LAST",	// end marker, do not add below here
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int GetTFCondId( const char *pszTFCondName )
{
	assert( ARRAYSIZE( g_aTFCondNames ) == ( TF_COND_LAST + 1 ) );

	for ( int iTFCond = 0; iTFCond < ARRAYSIZE( g_aTFCondNames ); ++iTFCond )
	{
		if ( !Q_stricmp( pszTFCondName, g_aTFCondNames[iTFCond] ) )
			return iTFCond;
	}

	return TF_COND_AIMING;
}