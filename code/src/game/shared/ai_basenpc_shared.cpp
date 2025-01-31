//=============================================================================//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "ai_basenpc_shared.h"
#include "tf_shareddefs.h"
#include "tf_fx_shared.h"
#include "effect_dispatch_data.h"
// Client specific.
#ifdef CLIENT_DLL
#include "engine/ivdebugoverlay.h"
#include "c_te_effect_dispatch.h"
#else // Server specific
#include "te_effect_dispatch.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#if defined(TF_CLASSIC) || defined(TF_CLASSIC_CLIENT)
TF_NPCData g_aNPCData[] =
{
	// Friendly actors.
	{
		"npc_dog",
		TF_TEAM_RED,
		TFFL_BUILDING,
	},
	{
		"npc_eli",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_fisherman",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_gman",
		TF_TEAM_RED,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_FIREPROOF | TFFL_NODEFLECT | TFFL_NOJAR | TFFL_NOBLEED,
	},
	{
		"npc_kleiner",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_magnusson",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_mossman",
		TF_TEAM_RED,
		0,
	},
	// Vital allies.
	{
		"npc_alyx",
		TF_TEAM_RED,
		TFFL_NOBACKSTAB,
	},
	{
		"npc_barney",
		TF_TEAM_RED,
		TFFL_NOBACKSTAB,
	},
	{
		"npc_monk",
		TF_TEAM_RED,
		TFFL_NOBACKSTAB,
	},
	// Regular allies.
	{
		"npc_citizen",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_vortigaunt",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_conscript",
		TF_TEAM_BLUE,
		0,
	},
	{
		"npc_conscriptred",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_crematorred",
		TF_TEAM_RED,
		0,
	},
	{
		"npc_combine_synth",
		TF_TEAM_BLUE,
		0,
	},
	{
		"monster_scientist",
		TF_TEAM_RED,
		0,
	},
	{
		"monster_barney",
		TF_TEAM_RED,
		0,
	},
	{
		"monster_bigmomma",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB | TFFL_NOHEALING,
	},
	{
		"monster_human_grunt",
		TF_TEAM_BLUE,
		0,
	},
	{
		"monster_robo_grunt",
		TF_TEAM_BLUE,
		0,
	},
	{
		"monster_alien_grunt",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_alien_slave",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_vortigaunt",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_headcrab",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_zombie",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_gonome",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_bullchicken",
		TF_TEAM_YELLOW,
		0,
	},
	{
		"monster_apache",
		TF_TEAM_BLUE,
		0,
	},
	{
		"monster_nihilanth",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_NODEFLECT | TFFL_NOJAR | TFFL_NOBLEED,
	},
	{
		"monster_hornet",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_snark",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB | TFFL_NOHEALING,
	},
	{
		"monster_alien_controller",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_gargantua",
		TF_TEAM_GREEN,
		0,
	},
	{
		"monster_ichthyosaur",
		TF_TEAM_YELLOW,
		0,
	},
	{
		"monster_barnacle",
		TF_TEAM_YELLOW,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_NODEFLECT,
	},
	{
		"monster_osprey",
		TF_TEAM_BLUE,
		0,
	},
	{
		"monster_sentry",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"monster_tentacle",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_NODEFLECT | TFFL_NOJAR | TFFL_NOBLEED,
	},
	{
		"monster_gman",
		TF_TEAM_RED,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_FIREPROOF | TFFL_NODEFLECT | TFFL_NOJAR | TFFL_NOBLEED,
	},
	{
		"monster_human_assassin",
		TF_TEAM_BLUE,
		0,
	},
	{
		"monster_leech",
		TF_TEAM_YELLOW,
		0,
	},
	{
		"monster_sitting_scientist",
		TF_TEAM_RED,
		0,
	},
	{
		"monster_houndeye",
		TF_TEAM_YELLOW,
		0,
	},
	// Enemy actors.
	{
		"npc_citizen",
		TF_TEAM_BLUE,
		0,
	},
	{
		"npc_breen",
		TF_TEAM_BLUE,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_FIREPROOF | TFFL_NODEFLECT | TFFL_NOJAR | TFFL_NOBLEED,
	},
	// Regular enemies.
	{
		"npc_combine_s",
		TF_TEAM_BLUE,
		0,
	},
	{
		"npc_metropolice",
		TF_TEAM_BLUE,
		0,
	},
	{
		"npc_stalker",
		TF_TEAM_BLUE,
		0,
	},
	{
		"npc_cremator",
		TF_TEAM_BLUE,
		0,
	},
	// Regular combine mechs.
	{
		"npc_cscanner",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	{
		"npc_clawscanner",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	{
		"npc_manhack",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	// Indestructible combine mechs.
	{
		"npc_combine_camera",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"npc_rollermine",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	{
		"npc_turret_ceiling",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"npc_turret_floor",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NOBACKSTAB,
	},
	{
		"npc_turret_ground",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	// Combine synths.
	{
		"npc_combinegunship",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"npc_hunter",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	{
		"npc_strider",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"npc_helicopter",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"npc_combinedropship",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	{
		"npc_combineguard",
		TF_TEAM_BLUE,
		TFFL_BUILDING | TFFL_NODEFLECT,
	},
	// Misc Combine NPCs.
	{
		"npc_crabsynth",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	{
		"npc_mortarsynth",
		TF_TEAM_BLUE,
		TFFL_BUILDING,
	},
	{
		"npc_sniper",
		TF_TEAM_BLUE,
		TFFL_NOHEALING | TFFL_NODEFLECT,
	},
	// Headcrabs.
	{
		"npc_headcrab",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB,
	},
	{
		"npc_headcrab_fast",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB,
	},
	{
		"npc_headcrab_black",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB,
	},
	{
		"npc_headcrab_poison",
		TF_TEAM_GREEN,
		TFFL_NOBACKSTAB,
	},
	// Zombies.
	{
		"npc_zombie",
		TF_TEAM_GREEN,
		0,
	},
	{
		"npc_zombie_torso",
		TF_TEAM_GREEN,
		0,
	},
	{
		"npc_fastzombie",
		TF_TEAM_GREEN,
		0,
	},
	{
		"npc_fastzombie_torso",
		TF_TEAM_GREEN,
		0,
	},
	{
		"npc_poisonzombie",
		TF_TEAM_GREEN,
		0,
	},
	{
		"npc_zombie_custom",
		TF_TEAM_GREEN,
		TFFL_NODEATHNOTICE,
	},
	{
		"npc_zombine",
		TF_TEAM_GREEN,
		0,
	},
	// Antlions.
	{
		"npc_antlion",
		TF_TEAM_YELLOW,
		TFFL_NOBACKSTAB,
	},
	{
		"npc_antlionguard",
		TF_TEAM_YELLOW,
		TFFL_NOBACKSTAB | TFFL_NODEFLECT,
	},
	// Neutral NPCs.
	{
		"npc_barnacle",
		TEAM_UNASSIGNED,
		TFFL_NOBACKSTAB | TFFL_NOHEALING | TFFL_NODEFLECT,
	},
	// Birds
	{
		"npc_crow",
		TEAM_UNASSIGNED,
		TFFL_NOBACKSTAB | TFFL_NOREWARD,
	},
	{
		"npc_pigeon",
		TEAM_UNASSIGNED,
		TFFL_NOBACKSTAB | TFFL_NOREWARD,
	},
	{
		"npc_seagull",
		TEAM_UNASSIGNED,
		TFFL_NOBACKSTAB | TFFL_NOREWARD,
	},
	{
		"generic_actor",
		TEAM_UNASSIGNED,
		0,
	},
	{
		"cyler_actor",
		TEAM_UNASSIGNED,
		0,
	},
	{
		"monster_furniture",
		TEAM_UNASSIGNED,
		TFFL_BUILDING | TFFL_NODEFLECT | TFFL_NODEATHNOTICE | TFFL_NOREWARD,
	},
	{
		"npc_furniture",
		TEAM_UNASSIGNED,
		TFFL_BUILDING | TFFL_NODEFLECT | TFFL_NODEATHNOTICE | TFFL_NOREWARD,
	},
	{
		"monster_generic",
		TEAM_UNASSIGNED,
		0,
	},
	{
		"npc_bullseye",
		TEAM_UNASSIGNED,
		TFFL_BUILDING | TFFL_NODEFLECT | TFFL_NODEATHNOTICE | TFFL_NOREWARD,
	},
	// End marker.
	{
		NULL,
		0,
		0,
	},
};

extern ConVar tf_max_health_boost;

//-----------------------------------------------------------------------------
// Purpose: Add a condition and duration
// duration of PERMANENT_CONDITION means infinite duration
//-----------------------------------------------------------------------------
void CAI_BaseNPC::AddCond( int nCond, float flDuration /* = PERMANENT_CONDITION */ )
{
#ifdef GAME_DLL
	Assert( nCond >= 0 && nCond < TF_COND_LAST );

	int nCondFlag = nCond;
	int *pVar = NULL;
	if ( nCond < 96 )
	{
		if ( nCond < 64 )
		{
			if ( nCond < 32 )
			{
				pVar = &m_nPlayerCond.GetForModify();
			}
			else
			{
				pVar = &m_nPlayerCondEx.GetForModify();
				nCondFlag -= 32;
			}
		}
		else
		{
			pVar = &m_nPlayerCondEx2.GetForModify();
			nCondFlag -= 64;
		}
	}
	else
	{
		pVar = &m_nPlayerCondEx3.GetForModify();
		nCondFlag -= 96;
	}

	*pVar |= ( 1 << nCondFlag );
	m_flCondExpireTimeLeft.Set( nCond, flDuration );
#endif
	OnConditionAdded( nCond );
}

//-----------------------------------------------------------------------------
// Purpose: Forcibly remove a condition
//-----------------------------------------------------------------------------
void CAI_BaseNPC::RemoveCond( int nCond )
{
#ifdef GAME_DLL
	Assert( nCond >= 0 && nCond < TF_COND_LAST );
	int nCondFlag = nCond;
	int *pVar = NULL;
	if ( nCond < 96 )
	{
		if ( nCond < 64 )
		{
			if ( nCond < 32 )
			{
				pVar = &m_nPlayerCond.GetForModify();
			}
			else
			{
				pVar = &m_nPlayerCondEx.GetForModify();
				nCondFlag -= 32;
			}
		}
		else
		{
			pVar = &m_nPlayerCondEx2.GetForModify();
			nCondFlag -= 64;
		}
	}
	else
	{
		pVar = &m_nPlayerCondEx3.GetForModify();
		nCondFlag -= 96;
	}

	*pVar &= ~( 1 << nCondFlag );
	m_flCondExpireTimeLeft.Set( nCond, 0 );
#endif
	OnConditionRemoved( nCond );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::InCond( int nCond )
{
	Assert( nCond >= 0 && nCond < TF_COND_LAST );
/*
	int nCondFlag = nCond;
	const int *pVar = NULL;

	if ( nCond < 96 )
	{
		if ( nCond < 64 )
		{
			if ( nCond < 32 )
			{
				pVar = &m_nPlayerCond.Get();
			}
			else
			{
				pVar = &m_nPlayerCondEx.Get();
				nCondFlag -= 32;
			}
		}
		else
		{
			pVar = &m_nPlayerCondEx2.Get();
			nCondFlag -= 64;
		}
	}
	else
	{
		pVar = &m_nPlayerCondEx3.Get();
		nCondFlag -= 96;
	}

	return ( ( *pVar & ( 1 << nCondFlag ) ) != 0 );
*/
	return ( ( m_nPlayerCond /*|| m_nPlayerCondEx || m_nPlayerCondEx2 || m_nPlayerCondEx3*/ & (1<<nCond) ) != 0 );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
float CAI_BaseNPC::GetConditionDuration( int nCond )
{
#ifdef GAME_DLL
	Assert( nCond >= 0 && nCond < TF_COND_LAST );

	if ( InCond( nCond ) )
	{
		return m_flCondExpireTimeLeft[nCond];
	}
#endif
	return 0.0f;
}

//-----------------------------------------------------------------------------
// Purpose: Remove any conditions affecting players
//-----------------------------------------------------------------------------
void CAI_BaseNPC::RemoveAllCond( void )
{
	int i;
	for ( i = 0; i < TF_COND_LAST; i++ )
	{
		if ( InCond( i ) )
		{
			RemoveCond( i );
		}
	}

	// Now remove all the rest
	m_nPlayerCond = 0;
	m_nPlayerCondEx = 0;
	m_nPlayerCondEx2 = 0;
	m_nPlayerCondEx3 = 0;
}


//-----------------------------------------------------------------------------
// Purpose: Called on both client and server. Server when we add the bit,
// and client when it recieves the new cond bits and finds one added
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnConditionAdded( int nCond )
{
	switch( nCond )
	{
	case TF_COND_HEALTH_BUFF:
#ifdef GAME_DLL
		m_flHealFraction = 0;
#endif
		break;

	case TF_COND_INVULNERABLE:
	case TF_COND_INVULNERABLE_USER_BUFF:
	case TF_COND_INVULNERABLE_CARD_EFFECT:
		OnAddInvulnerable();
		break;

	case TF_COND_BURNING:
		OnAddBurning();
		break;

	case TF_COND_BLEEDING:
	case TF_COND_GRAPPLINGHOOK_BLEEDING:
		OnAddBleeding();
		break;

	case TF_COND_PHASE:
		OnAddPhase();
		break;

	case TF_COND_HEALTH_OVERHEALED:
		//UpdateOverhealEffect();
		break;
#ifdef GAME_DLL
	case TF_COND_SLOWED:
		OnAddSlowed();
		break;
#endif
	case TF_COND_CRITBOOSTED:
	case TF_COND_CRITBOOSTED_PUMPKIN:
	case TF_COND_CRITBOOSTED_USER_BUFF:
	case TF_COND_CRITBOOSTED_DEMO_CHARGE:
	case TF_COND_CRITBOOSTED_FIRST_BLOOD:
	case TF_COND_CRITBOOSTED_BONUS_TIME:
	case TF_COND_CRITBOOSTED_CTF_CAPTURE:
	case TF_COND_CRITBOOSTED_ON_KILL:
	case TF_COND_CRITBOOSTED_CARD_EFFECT:
	case TF_COND_CRITBOOSTED_RUNE_TEMP:
	//case TF_COND_NOHEALINGDAMAGEBUFF: // this one doesn't have spark effect.
	case TF_COND_MINICRITBOOSTED_ON_KILL:
		OnAddCritboosted();
		break;
#ifdef GAME_DLL
	case TF_COND_HALLOWEEN_GIANT:
		OnAddHalloweenGiant();
		break;

	case TF_COND_HALLOWEEN_TINY:
		OnAddHalloweenTiny();
		break;
#endif
	case TF_COND_STUNNED:
		OnAddStunned();
		break;
#ifdef CLIENT_DLL
	case TF_COND_URINE:
		OnAddUrine();
		break;

	case TF_COND_MAD_MILK:
		OnAddMadMilk();
		break;

	case TF_COND_GAS:
		OnAddCondGas();
		break;
#endif
	case TF_COND_SPEED_BOOST:
	case TF_COND_HALLOWEEN_SPEED_BOOST:
		OnAddSpeedBoost();
		break;
#ifdef GAME_DLL
	case TF_COND_RUNE_STRENGTH:
	case TF_COND_RUNE_HASTE:
	case TF_COND_RUNE_REGEN:
	case TF_COND_RUNE_RESIST:
	case TF_COND_RUNE_VAMPIRE:
	case TF_COND_RUNE_WARLOCK:
	case TF_COND_RUNE_PRECISION:
	case TF_COND_RUNE_AGILITY:
	case TF_COND_RUNE_KNOCKOUT:
	case TF_COND_RUNE_KING:
	case TF_COND_RUNE_PLAGUE:
	case TF_COND_RUNE_SUPERNOVA:
		OnAddRune();
		break;
#else
	case TF_COND_OFFENSEBUFF:
	case TF_COND_DEFENSEBUFF:
	case TF_COND_REGENONDAMAGEBUFF:
		OnAddBuff();
		break;
#endif

	case TF_COND_SAPPED:
		OnAddSapped();
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Called on both client and server. Server when we remove the bit,
// and client when it recieves the new cond bits and finds one removed
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnConditionRemoved( int nCond )
{
	switch( nCond )
	{
	case TF_COND_BURNING:
		OnRemoveBurning();
		break;

	case TF_COND_BLEEDING:
	case TF_COND_GRAPPLINGHOOK_BLEEDING:
		OnRemoveBleeding();
		break;

	case TF_COND_PHASE:
		OnRemovePhase();
		break;

	case TF_COND_HEALTH_BUFF:
#ifdef GAME_DLL
		m_flHealFraction = 0;
#endif
		break;

	case TF_COND_HEALTH_OVERHEALED:
		//UpdateOverhealEffect();
		break;
#ifdef GAME_DLL
	case TF_COND_SLOWED:
		OnRemoveSlowed();
		break;
#endif
	case TF_COND_INVULNERABLE:
	case TF_COND_INVULNERABLE_USER_BUFF:
	case TF_COND_INVULNERABLE_CARD_EFFECT:
		OnRemoveInvulnerable();
		break;

	case TF_COND_CRITBOOSTED:
	case TF_COND_CRITBOOSTED_PUMPKIN:
	case TF_COND_CRITBOOSTED_USER_BUFF:
	case TF_COND_CRITBOOSTED_DEMO_CHARGE:
	case TF_COND_CRITBOOSTED_FIRST_BLOOD:
	case TF_COND_CRITBOOSTED_BONUS_TIME:
	case TF_COND_CRITBOOSTED_CTF_CAPTURE:
	case TF_COND_CRITBOOSTED_ON_KILL:
	case TF_COND_CRITBOOSTED_CARD_EFFECT:
	case TF_COND_CRITBOOSTED_RUNE_TEMP:
	//case TF_COND_NOHEALINGDAMAGEBUFF: // this one doesn't have spark effect.
	case TF_COND_MINICRITBOOSTED_ON_KILL:
		OnRemoveCritboosted();
		break;
#ifdef GAME_DLL
	case TF_COND_HALLOWEEN_GIANT:
		OnRemoveHalloweenGiant();
		break;

	case TF_COND_HALLOWEEN_TINY:
		OnRemoveHalloweenTiny();
		break;
#endif
	case TF_COND_STUNNED:
		OnRemoveStunned();
		break;

	case TF_COND_URINE:
		OnRemoveUrine();
		break;

	case TF_COND_MAD_MILK:
		OnRemoveMadMilk();
		break;

	case TF_COND_GAS:
		OnRemoveCondGas();
		break;

	case TF_COND_SPEED_BOOST:
	case TF_COND_HALLOWEEN_SPEED_BOOST:
		OnRemoveSpeedBoost();
		break;
#ifdef GAME_DLL
	case TF_COND_RUNE_STRENGTH:
	case TF_COND_RUNE_HASTE:
	case TF_COND_RUNE_REGEN:
	case TF_COND_RUNE_RESIST:
	case TF_COND_RUNE_VAMPIRE:
	case TF_COND_RUNE_WARLOCK:
	case TF_COND_RUNE_PRECISION:
	case TF_COND_RUNE_AGILITY:
	case TF_COND_RUNE_KNOCKOUT:
	case TF_COND_RUNE_KING:
	case TF_COND_RUNE_PLAGUE:
	case TF_COND_RUNE_SUPERNOVA:
		OnRemoveRune();
		break;
#else
	case TF_COND_OFFENSEBUFF:
	case TF_COND_DEFENSEBUFF:
	case TF_COND_REGENONDAMAGEBUFF:
		OnRemoveBuff();
		break;
#endif

	case TF_COND_SAPPED:
		OnRemoveSapped();
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsCritBoosted( void )
{
	// Oh man...
	if ( InCond( TF_COND_CRITBOOSTED ) ||
		InCond( TF_COND_CRITBOOSTED_PUMPKIN ) ||
		InCond( TF_COND_CRITBOOSTED_USER_BUFF ) ||
		InCond( TF_COND_CRITBOOSTED_DEMO_CHARGE ) ||
		InCond( TF_COND_CRITBOOSTED_FIRST_BLOOD ) ||
		InCond( TF_COND_CRITBOOSTED_BONUS_TIME ) ||
		InCond( TF_COND_CRITBOOSTED_CTF_CAPTURE ) ||
		InCond( TF_COND_CRITBOOSTED_ON_KILL ) ||
		InCond( TF_COND_CRITBOOSTED_CARD_EFFECT ) ||
		InCond( TF_COND_CRITBOOSTED_RUNE_TEMP ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsMiniCritBoosted( void )
{
	if ( InCond( TF_COND_NOHEALINGDAMAGEBUFF ) ||
		InCond( TF_COND_MINICRITBOOSTED_ON_KILL ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsInvulnerable( void )
{
	// Oh man again...
	if ( InCond( TF_COND_INVULNERABLE ) ||
		InCond( TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGE ) ||
		InCond( TF_COND_INVULNERABLE_USER_BUFF ) ||
		InCond( TF_COND_INVULNERABLE_CARD_EFFECT ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsStealthed( void )
{
	if ( InCond( TF_COND_STEALTHED ) ||
		InCond( TF_COND_STEALTHED_USER_BUFF ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsJared( void )
{
	if ( InCond( TF_COND_URINE ) ||
		InCond( TF_COND_MAD_MILK ) ||
		InCond( TF_COND_GAS ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsSpeedBoosted( void )
{
	if ( InCond( TF_COND_SPEED_BOOST ) ||
		InCond( TF_COND_HALLOWEEN_SPEED_BOOST ) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CAI_BaseNPC::IsBuffed( void )
{
	if ( InCond( TF_COND_OFFENSEBUFF ) ||
		InCond( TF_COND_DEFENSEBUFF ) || 
		InCond( TF_COND_REGENONDAMAGEBUFF ) )
		return true;

	return false;
}

int CAI_BaseNPC::GetMaxBuffedHealth( void )
{
	float flBoostMax = GetMaxHealth() * tf_max_health_boost.GetFloat();
	CALL_ATTRIB_HOOK_FLOAT( flBoostMax, mult_medigun_overheal_amount );
	int iRoundDown = floor( flBoostMax / 5 );
	iRoundDown = iRoundDown * 5;

	return iRoundDown;
}
	extern ConVar	tf_fireball_burn_duration;
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::Burn( CTFPlayer *pAttacker, CTFWeaponBase *pWeapon /*= NULL*/, float flFlameDuration /*= -1.0f*/ )
{
#ifdef CLIENT_DLL

#else
	// Don't bother igniting players who have just been killed by the fire damage.
	if ( !IsAlive() )
		return;

	if ( !InCond( TF_COND_BURNING ) )
	{
		if ( AllowedToIgnite() == true )
		{
			// Start burning
			AddCond( TF_COND_BURNING );
			m_flFlameBurnTime = gpGlobals->curtime;	//asap
		}
		// let the attacker know he burned me
		if ( pAttacker  )
		{
			pAttacker->OnBurnOther( this );
		}
	}

	float flFlameLife = TF_BURNING_FLAME_LIFE;

	if ( flFlameDuration != -1.0f )
		flFlameLife = flFlameDuration;

	CALL_ATTRIB_HOOK_FLOAT_ON_OTHER( pAttacker, flFlameLife, mult_wpn_burntime );

	if ( pWeapon && !pWeapon->IsWeapon( TF_WEAPON_ROCKETLAUNCHER_FIREBALL ) )
	{
		m_flFlameRemoveTime = gpGlobals->curtime + flFlameLife;
	}
	else
	{
		// dragon's fury afterburn is 2 second
		m_flFlameRemoveTime = gpGlobals->curtime + tf_fireball_burn_duration.GetFloat();
	}

	m_hBurnAttacker = pAttacker;
	m_hBurnWeapon = pWeapon;

#endif
}

//-----------------------------------------------------------------------------
// Purpose: BLOOD LEAKING
//-----------------------------------------------------------------------------
void CAI_BaseNPC::MakeBleed( CTFPlayer *pAttacker, CTFWeaponBase *pWeapon /*= NULL*/, float flBleedDuration /*= -1.0f*/ )
{
#ifdef CLIENT_DLL

#else
	// Don't bother bleeding npc who have just been killed by the bleed damage.
	if ( !IsAlive() )
		return;

	if ( !InCond( TF_COND_BLEEDING ) )
	{
		// Start bleeding
		AddCond( TF_COND_BLEEDING );
		m_flBleedTime = gpGlobals->curtime;
	}

	float flBleedTime = TF_BLEEDING_TIME;

	if ( flBleedDuration != -1.0f )
		flBleedTime = flBleedDuration;

	m_flBleedRemoveTime = gpGlobals->curtime + flBleedTime;

	m_hBleedAttacker = pAttacker;
	m_hBleedWeapon = pWeapon;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddBurning( void )
{
#ifdef CLIENT_DLL
	// Start the burning effect
	if ( !m_pBurningEffect )
	{
		const char *pEffectName = ( GetTeamNumber() == TF_TEAM_BLUE ) ? "burningplayer_blue" : "burningplayer_red";
		m_pBurningEffect = ParticleProp()->Create( pEffectName, PATTACH_ABSORIGIN_FOLLOW );

		m_flBurnEffectStartTime = gpGlobals->curtime;
		m_flBurnEffectEndTime = gpGlobals->curtime + TF_BURNING_FLAME_LIFE;
	}
#endif

	// play a fire-starting sound
	EmitSound( "Fire.Engulf" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveBurning( void )
{
#ifdef CLIENT_DLL
	StopBurningSound();

	if ( m_pBurningEffect )
	{
		ParticleProp()->StopEmission( m_pBurningEffect );
		m_pBurningEffect = NULL;
	}

	m_flBurnEffectStartTime = 0;
	m_flBurnEffectEndTime = 0;
#else
	m_hBurnAttacker = NULL;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddBleeding( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveBleeding( void )
{
#ifdef GAME_DLL
	m_hBleedAttacker = NULL;
#endif
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddInvulnerable( void )
{
	// Stock uber removes negative conditions.
	if ( InCond( TF_COND_BURNING ) )
	{
		RemoveCond( TF_COND_BURNING );
	}

	if ( InCond( TF_COND_BLEEDING ) )
	{
		RemoveCond( TF_COND_BLEEDING );
	}

	if ( InCond( TF_COND_SLOWED ) )
	{
		RemoveCond( TF_COND_SLOWED );
	}

	if ( IsJared() )
	{
		RemoveCond( TF_COND_URINE );
		RemoveCond( TF_COND_MAD_MILK );
		RemoveCond( TF_COND_GAS );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveInvulnerable( void )
{
}
#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddSlowed( void )
{
	SetPlaybackRate( 0.6f );
}

//-----------------------------------------------------------------------------
// Purpose: Remove slowdown effect
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveSlowed( void )
{
	SetPlaybackRate( 1.0f );
}
#endif
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddCritboosted( void )
{
#ifdef CLIENT_DLL
	UpdateCritBoostEffect();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveCritboosted( void )
{
#ifdef CLIENT_DLL
	UpdateCritBoostEffect();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddStunned( void )
{
#ifdef GAME_DLL
	SetPlaybackRate( 0.0f );
#else
	if ( !m_pStun )
		m_pStun = ParticleProp()->Create( "conc_stars", PATTACH_POINT_FOLLOW, "head" );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveStunned( void )
{
	/*m_flStunExpireTime = 0.0f;
	m_hStunner = NULL;
	m_iStunPhase = 0;*/

#ifdef GAME_DLL
	SetPlaybackRate( 1.0f );
#else
	ParticleProp()->StopEmission( m_pStun );
	m_pStun = NULL;
#endif
}
#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddHalloweenGiant( void )
{
	SetModelScale( 2.0, 0.0 );

	SetMaxHealth( GetMaxHealth() * 10 );
	SetHealth( GetMaxHealth() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveHalloweenGiant( void )
{
	SetModelScale( 1.0, 0.0 );

	SetMaxHealth( GetMaxHealth() );
	SetHealth( GetMaxHealth() );

}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddPhase(void)
{
	UpdatePhaseEffects();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemovePhase(void)
{
#ifdef GAME_DLL
	for ( int i = 0; i < m_pPhaseTrails.Count(); i++ )
	{
		m_pPhaseTrails[i]->SUB_Remove();
	}
	m_pPhaseTrails.RemoveAll();
#else
	ParticleProp()->StopEmission( m_pWarp );
	m_pWarp = NULL;
#endif
}
#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddHalloweenTiny( void )
{
	SetModelScale( 0.5, 0.0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveHalloweenTiny( void )
{

	SetModelScale( 1.0, 0.0 );
}

//-----------------------------------------------------------------------------
// Purpose: Bonk phase effects
//-----------------------------------------------------------------------------
void CAI_BaseNPC::AddPhaseEffects(void)
{
	const char* pszEffect = GetTeamNumber() == TF_TEAM_BLUE ? "effects/beam001_blu.vmt" : "effects/beam001_red.vmt";
	Vector vecOrigin = GetAbsOrigin();
	
	/*CSpriteTrail *pPhaseTrail = CSpriteTrail::SpriteTrailCreate( pszEffect, vecOrigin, true );
	pPhaseTrail->SetTransparency( kRenderTransAlpha, 255, 255, 255, 255, 0 );
	pPhaseTrail->SetStartWidth( 12.0f );
	pPhaseTrail->SetTextureResolution( 0.01416667 );
	pPhaseTrail->SetLifeTime( 1.0 );
	pPhaseTrail->SetAttachment( this, LookupAttachment( "back_upper" ) );
	m_pPhaseTrails.AddToTail( pPhaseTrail );

	pPhaseTrail = CSpriteTrail::SpriteTrailCreate( pszEffect, vecOrigin, true );
	pPhaseTrail->SetTransparency( kRenderTransAlpha, 255, 255, 255, 255, 0 );
	pPhaseTrail->SetStartWidth( 16.0f );
	pPhaseTrail->SetTextureResolution( 0.01416667 );
	pPhaseTrail->SetLifeTime( 1.0 );
	pPhaseTrail->SetAttachment( this, LookupAttachment( "back_lower" ) );
	m_pPhaseTrails.AddToTail( pPhaseTrail );

	// White trail for socks
	pPhaseTrail = CSpriteTrail::SpriteTrailCreate( "effects/beam001_white.vmt", vecOrigin, true );
	pPhaseTrail->SetTransparency( kRenderTransAlpha, 255, 255, 255, 255, 0 );
	pPhaseTrail->SetStartWidth( 8.0f );
	pPhaseTrail->SetTextureResolution( 0.01416667 );
	pPhaseTrail->SetLifeTime( 0.5 );
	pPhaseTrail->SetAttachment( this, LookupAttachment( "foot_R" ) );
	m_pPhaseTrails.AddToTail( pPhaseTrail );

	pPhaseTrail = CSpriteTrail::SpriteTrailCreate( "effects/beam001_white.vmt", vecOrigin, true );
	pPhaseTrail->SetTransparency( kRenderTransAlpha, 255, 255, 255, 255, 0 );
	pPhaseTrail->SetStartWidth( 8.0f );
	pPhaseTrail->SetTextureResolution( 0.01416667 );
	pPhaseTrail->SetLifeTime( 0.5 );
	pPhaseTrail->SetAttachment( this, LookupAttachment( "foot_L" ) );
	m_pPhaseTrails.AddToTail( pPhaseTrail );*/

	CSpriteTrail *pPhaseTrail = CSpriteTrail::SpriteTrailCreate( pszEffect, vecOrigin, true );
	pPhaseTrail->SetTransparency( kRenderTransAlpha, 255, 255, 255, 255, 0 );
	pPhaseTrail->SetStartWidth( 16.0f );
	pPhaseTrail->SetTextureResolution( 0.01416667 );
	pPhaseTrail->SetLifeTime( 1.0 );
	pPhaseTrail->SetAttachment( this, LookupAttachment( "chest" ) );
	m_pPhaseTrails.AddToTail( pPhaseTrail );
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Update phase effects
//-----------------------------------------------------------------------------
void CAI_BaseNPC::UpdatePhaseEffects(void)
{
	if ( !InCond( TF_COND_PHASE ) )
		return;

#ifdef CLIENT_DLL
	if( GetAbsVelocity() != vec3_origin )
	{
		if ( !m_pWarp )
		{
			m_pWarp = ParticleProp()->Create( "warp_version", PATTACH_ABSORIGIN_FOLLOW );
		}
	}
#else
	if ( m_pPhaseTrails.IsEmpty() )
	{
		AddPhaseEffects();
	}
		
	// Turn on the trails if they're not active already
	if ( m_pPhaseTrails[0] && !m_pPhaseTrails[0]->IsOn() )
	{
		for( int i = 0; i < m_pPhaseTrails.Count(); i++ )
		{
			m_pPhaseTrails[i]->TurnOn();
		}
	}
#endif
}
#ifdef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: Update speedboost effects
//-----------------------------------------------------------------------------
void CAI_BaseNPC::UpdateSpeedBoostEffects(void)
{
	if ( IsSpeedBoosted() )
	{
		if(  GetAbsVelocity() != vec3_origin )
		{
			// We're on the move
			if ( !m_pSpeedTrails )
			{
				m_pSpeedTrails = ParticleProp()->Create( "speed_boost_trail", PATTACH_ABSORIGIN_FOLLOW );
			}
		}
		else
		{
			// We're not moving
			if( m_pSpeedTrails )
			{
				ParticleProp()->StopEmission( m_pSpeedTrails );
				m_pSpeedTrails = NULL;
			}
		}
	}
	else
	{
		ParticleProp()->StopEmission( m_pSpeedTrails );
		m_pSpeedTrails = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddUrine( void )
{
	ParticleProp()->Create( "peejar_drips", PATTACH_ABSORIGIN_FOLLOW ); 
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddMadMilk( void )
{
	ParticleProp()->Create( "peejar_drips_milk", PATTACH_ABSORIGIN_FOLLOW );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddCondGas( void )
{
	const char *pszEffectName = ConstructTeamParticle( "gas_can_drips_%s", GetTeamNumber() );
	ParticleProp()->Create( pszEffectName, PATTACH_ABSORIGIN_FOLLOW ); 
}
#endif
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveUrine( void )
{
#ifdef GAME_DLL
	if( IsAlive() )
	{
		m_hUrineAttacker = NULL;
	}
#else
	ParticleProp()->StopParticlesNamed( "peejar_drips" );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveMadMilk( void )
{
#ifdef GAME_DLL
	if( IsAlive() )
	{
		m_hUrineAttacker = NULL;
	}
#else
	ParticleProp()->StopParticlesNamed( "peejar_drips_milk" );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveCondGas( void )
{
#ifdef GAME_DLL
	if( IsAlive() )
	{
		m_hUrineAttacker = NULL;
	}
#else
	ParticleProp()->StopParticlesNamed( "gas_can_drips_red" );
	ParticleProp()->StopParticlesNamed( "gas_can_drips_blue" );
#endif
}

#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddRune( void )
{


}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveRune( void )
{

}
#endif

#ifdef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddBuff( void )
{
	// Start the buff effect
	if ( !m_pBuffAura )
	{
		const char *pszEffectName = ConstructTeamParticle( "soldierbuff_%s_buffed", GetTeamNumber() );

		m_pBuffAura = ParticleProp()->Create( pszEffectName, PATTACH_ABSORIGIN_FOLLOW );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveBuff( void )
{
	if ( m_pBuffAura )
	{
		ParticleProp()->StopEmission( m_pBuffAura );
		m_pBuffAura = NULL;
	}
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddSapped( void )
{
#ifdef GAME_DLL
	EmitSound( "Weapon_Sapper.Plant" );
	//EmitSound( "Weapon_Sapper.Timer" );
	m_flSappedDamageAccumulator = 0;
	m_flLastSappedThinkTime = gpGlobals->curtime;
	SetContextThink( &CAI_BaseNPC::SappedThink, gpGlobals->curtime + 0.1, "SappedThink" );
#else
	if ( !m_pSapped )
	{
		m_pSapped = ParticleProp()->Create( "sapper_sentry1_fx", PATTACH_ABSORIGIN_FOLLOW );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveSapped( void )
{
#ifdef GAME_DLL
	//StopSound( "Weapon_Sapper.Timer" );
	m_flLastSappedThinkTime = 0;
	SetContextThink( NULL, 0, "SappedThink" );
#else
	if ( m_pSapped )
	{
		ParticleProp()->StopEmission( m_pSapped );
		m_pSapped = NULL;
	}
#endif
}

#ifdef GAME_DLL
extern ConVar obj_sapper_amount;
//-----------------------------------------------------------------------------
// Purpose: Slowly drain hp from the npc I'm attached to
//-----------------------------------------------------------------------------
void CAI_BaseNPC::SappedThink( void )
{
	SetNextThink( gpGlobals->curtime + 0.1, "SappedThink" );

	// Don't bring npc back from the dead
	if ( !IsAlive() )
		return;

	// how much damage to give this think?
	float flTimeSinceLastThink = gpGlobals->curtime - m_flLastSappedThinkTime;
	float flDamageToGive = ( flTimeSinceLastThink ) * obj_sapper_amount.GetFloat();

	// add to accumulator
	m_flSappedDamageAccumulator += flDamageToGive;

	int iDamage = (int)m_flSappedDamageAccumulator;

	m_flSappedDamageAccumulator -= iDamage;

	CTakeDamageInfo info;
	info.SetDamage( iDamage );
	info.SetAttacker( this );
	info.SetInflictor( this );
	info.SetDamageType( DMG_CRUSH );

	TakeDamage( info );

	m_flLastSappedThinkTime = gpGlobals->curtime;
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnAddSpeedBoost( void )
{
#ifdef GAME_DLL
	SetPlaybackRate( 1.5f );
#else
	UpdateSpeedBoostEffects();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::OnRemoveSpeedBoost( void )
{
#ifdef GAME_DLL
	SetPlaybackRate( 1.0f );
#else
	UpdateSpeedBoostEffects();
#endif
}

extern ConVar tf_debug_bullets;
extern ConVar tf_useparticletracers;
extern ConVar sv_showimpacts;

//-----------------------------------------------------------------------------
// Purpose:
//   Input: info
//          bDoEffects - effects (blood, etc.) should only happen client-side.
//-----------------------------------------------------------------------------
void CAI_BaseNPC::FireBullet( const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType /*= TF_DMG_CUSTOM_NONE*/ )
{
	// Fire a bullet (ignoring the shooter).
	Vector vecStart = info.m_vecSrc;
	Vector vecEnd = vecStart + info.m_vecDirShooting * info.m_flDistance;
	trace_t trace;
	UTIL_TraceLine( vecStart, vecEnd, ( MASK_SHOT ), this, COLLISION_GROUP_NONE, &trace );

#ifdef GAME_DLL
	if ( tf_debug_bullets.GetBool() )
	{
		NDebugOverlay::Line( vecStart, trace.endpos, 0, 255, 0, true, 30 );
	}
#endif


#ifdef CLIENT_DLL
	if ( sv_showimpacts.GetInt() == 1 || sv_showimpacts.GetInt() == 2 )
	{
		// draw red client impact markers
		debugoverlay->AddBoxOverlay( trace.endpos, Vector(-2,-2,-2), Vector(2,2,2), QAngle( 0, 0, 0), 255,0,0,127, 4 );

		if ( trace.m_pEnt && trace.m_pEnt->IsPlayer() )
		{
			C_AI_BaseNPC *pNPC = dynamic_cast<C_AI_BaseNPC *>( trace.m_pEnt );
			pNPC->DrawClientHitboxes( 4, true );
		}
	}
#else
	if ( sv_showimpacts.GetInt() == 1 || sv_showimpacts.GetInt() == 3 )
	{
		// draw blue server impact markers
		NDebugOverlay::Box( trace.endpos, Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 0, 0, 255, 127, 4 );

		if ( trace.m_pEnt && trace.m_pEnt->IsPlayer() )
		{
			CAI_BaseNPC *pNPC = dynamic_cast<CAI_BaseNPC *>( trace.m_pEnt );
			pNPC->DrawServerHitboxes( 4, true );
		}
	}
#endif

	if ( trace.fraction < 1.0 )
	{
		// Verify we have an entity at the point of impact.
		Assert( trace.m_pEnt );

		if ( bDoEffects )
		{
			// If shot starts out of water and ends in water
			if ( !( enginetrace->GetPointContents( trace.startpos ) & ( CONTENTS_WATER | CONTENTS_SLIME ) ) &&
				( enginetrace->GetPointContents( trace.endpos ) & ( CONTENTS_WATER | CONTENTS_SLIME ) ) )
			{
				// Water impact effects.
				ImpactWaterTrace( trace, vecStart );
			}
			else
			{
				// Regular impact effects.

				// don't decal your teammates or objects on your team
				if ( trace.m_pEnt->GetTeamNumber() != GetTeamNumber() )
				{
					UTIL_ImpactTrace( &trace, nDamageType );
				}
			}

#ifdef CLIENT_DLL
			static int	tracerCount;
			if ( ( info.m_iTracerFreq != 0 ) && ( tracerCount++ % info.m_iTracerFreq ) == 0 )
			{
				// if this is a local player, start at attachment on view model
				// else start on attachment on weapon model

				int iEntIndex = entindex();
				int iUseAttachment = TRACER_DONT_USE_ATTACHMENT;
				int iAttachment = 1;

				C_BaseCombatWeapon *pWeapon = GetActiveWeapon();

				if( pWeapon )
					iAttachment = pWeapon->LookupAttachment( "muzzle" );

				bool bInToolRecordingMode = clienttools->IsInRecordingMode();

				if( pWeapon )
				{
					iEntIndex = pWeapon->entindex();

					int nModelIndex = pWeapon->GetModelIndex();
					int nWorldModelIndex = pWeapon->GetWorldModelIndex();
					if ( bInToolRecordingMode && nModelIndex != nWorldModelIndex )
					{
						pWeapon->SetModelIndex( nWorldModelIndex );
					}

					pWeapon->GetAttachment( iAttachment, vecStart );

					if ( bInToolRecordingMode && nModelIndex != nWorldModelIndex )
					{
						pWeapon->SetModelIndex( nModelIndex );
					}
				}

				if ( tf_useparticletracers.GetBool() )
				{
					const char *pszTracerEffect = GetTracerType();
					if ( pszTracerEffect && pszTracerEffect[0] )
					{
						char szTracerEffect[128];
						if ( nDamageType & DMG_CRITICAL )
						{
							Q_snprintf( szTracerEffect, sizeof(szTracerEffect), "%s_crit", pszTracerEffect );
							pszTracerEffect = szTracerEffect;
						}

						FX_TFTracer( pszTracerEffect, vecStart, trace.endpos, entindex(), true );
					}
				}
				else
				{
					UTIL_Tracer( vecStart, trace.endpos, entindex(), iUseAttachment, 5000, true, GetTracerType() );
				}
			}
#endif

		}

		// Server specific.
#ifndef CLIENT_DLL
		// See what material we hit.
		CTakeDamageInfo dmgInfo( this, info.m_pAttacker, GetActiveWeapon(), info.m_flDamage, nDamageType, nCustomDamageType );
		CalculateBulletDamageForce( &dmgInfo, info.m_iAmmoType, info.m_vecDirShooting, trace.endpos, 1.0 );	//MATTTODO bullet forces
		trace.m_pEnt->DispatchTraceAttack( dmgInfo, info.m_vecDirShooting, &trace );
#endif
	}
}

#ifdef CLIENT_DLL
extern ConVar tf_impactwatertimeenable;
extern ConVar tf_impactwatertime;
#endif

//-----------------------------------------------------------------------------
// Purpose: Trace from the shooter to the point of impact (another player,
//          world, etc.), but this time take into account water/slime surfaces.
//   Input: trace - initial trace from player to point of impact
//          vecStart - starting point of the trace 
//-----------------------------------------------------------------------------
void CAI_BaseNPC::ImpactWaterTrace( trace_t &trace, const Vector &vecStart )
{
#ifdef CLIENT_DLL
	if ( tf_impactwatertimeenable.GetBool() )
	{
		if ( m_flWaterImpactTime > gpGlobals->curtime )
			return;
	}
#endif 

	trace_t traceWater;
	UTIL_TraceLine( vecStart, trace.endpos, ( MASK_SHOT | CONTENTS_WATER | CONTENTS_SLIME ),
		this, COLLISION_GROUP_NONE, &traceWater );
	if ( traceWater.fraction < 1.0f )
	{
		CEffectData	data;
		data.m_vOrigin = traceWater.endpos;
		data.m_vNormal = traceWater.plane.normal;
		data.m_flScale = random->RandomFloat( 8, 12 );
		if ( traceWater.contents & CONTENTS_SLIME )
		{
			data.m_fFlags |= FX_WATER_IN_SLIME;
		}

		const char *pszEffectName = "tf_gunshotsplash";

		DispatchEffect( pszEffectName, data );

#ifdef CLIENT_DLL
		if ( tf_impactwatertimeenable.GetBool() )
		{
			m_flWaterImpactTime = gpGlobals->curtime + tf_impactwatertime.GetFloat();
		}
#endif
	}
}

//-----------------------------------------------------------------------------
// Purpose: Add Mannpower Revenge Crit
//-----------------------------------------------------------------------------
void CAI_BaseNPC::AddTempCritBonus( float flDuration )
{
	AddCond( TF_COND_RUNE_IMBALANCE, flDuration );
}