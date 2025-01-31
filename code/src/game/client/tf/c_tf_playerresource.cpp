//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: TF's custom C_PlayerResource
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_tf_playerresource.h"
#include <shareddefs.h>
#include <tf_shareddefs.h>
#include "hud.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


IMPLEMENT_CLIENTCLASS_DT( C_TF_PlayerResource, DT_TFPlayerResource, CTFPlayerResource )
	RecvPropArray3( RECVINFO_ARRAY( m_iTotalScore ), RecvPropInt( RECVINFO( m_iTotalScore[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iMaxHealth ), RecvPropInt( RECVINFO( m_iMaxHealth[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iPlayerClass ), RecvPropInt( RECVINFO( m_iPlayerClass[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_vecColors ), RecvPropVector( RECVINFO( m_vecColors[0] ) ) ),
	RecvPropArray3( RECVINFO_ARRAY( m_iKillstreak ), RecvPropInt( RECVINFO( m_iKillstreak[0] ) ) ),
END_RECV_TABLE()


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TF_PlayerResource::C_TF_PlayerResource()
{
	m_Colors[TEAM_UNASSIGNED] = COLOR_TF_SPECTATOR;
	m_Colors[TEAM_SPECTATOR] = COLOR_TF_SPECTATOR;
	m_Colors[TF_TEAM_RED] = COLOR_RED;
	m_Colors[TF_TEAM_BLUE] = COLOR_BLUE;
	m_Colors[TF_TEAM_GREEN] = COLOR_GREEN;
	m_Colors[TF_TEAM_YELLOW] = COLOR_YELLOW;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TF_PlayerResource::~C_TF_PlayerResource()
{
}

//-----------------------------------------------------------------------------
// Purpose: Gets a value from an array member
//-----------------------------------------------------------------------------
int C_TF_PlayerResource::GetArrayValue( int iIndex, int *pArray, int iDefaultVal )
{
	if ( !IsConnected( iIndex ) )
	{
		return iDefaultVal;
	}
	return pArray[iIndex];
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const Vector &C_TF_PlayerResource::GetPlayerColorVector( int iIndex )
{
	if ( !IsConnected( iIndex ) )
	{
		// White color.
		static Vector vecWhite( 1, 1, 1 );
		return vecWhite;
	}

	return m_vecColors[iIndex];
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color C_TF_PlayerResource::GetPlayerColor( int iIndex )
{
	const Vector &vecColor = GetPlayerColorVector( iIndex );

	return Color( vecColor.x * 255.0f, vecColor.y * 255.0f, vecColor.z * 255.0f, 255 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int C_TF_PlayerResource::GetCountForPlayerClass( int iTeam, int iClass, bool bExcludeLocalPlayer /*=false*/ )
{
	int count = 0;
	int iLocalPlayerIndex = GetLocalPlayerIndex();

	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		if ( bExcludeLocalPlayer && ( i == iLocalPlayerIndex ) )
		{
			continue;
		}

		if ( ( GetTeam( i ) == iTeam ) && ( GetPlayerClass( i ) == iClass ) )
		{
			count++;
		}
	}

	return count;
}

float C_TF_PlayerResource::GetHealthPercent()
{
	float max_health = 0;
	float health = 0;
	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		max_health = GetMaxHealth( i );
		if ( max_health <= 0 )
			return 0;

		health = GetHealth( i );
	}
	return clamp(health / max_health, 0.0f, 1.0f);
}