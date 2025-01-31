//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: Client side C_TFTeam class
//
// $NoKeywords: $
//=============================================================================
#include "cbase.h"
#include "engine/IEngineSound.h"
#include "hud.h"
#include "recvproxy.h"
#include "c_tf_team.h"
#include "tf_shareddefs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: RecvProxy that converts the Player's object UtlVector to entindexes
//-----------------------------------------------------------------------------
void RecvProxy_TeamObjectList( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	C_TFTeam *pPlayer = (C_TFTeam*)pStruct;
	CBaseHandle *pHandle = (CBaseHandle*)(&(pPlayer->m_aObjects[pData->m_iElement])); 
	RecvProxy_IntToEHandle( pData, pStruct, pHandle );
}

void RecvProxyArrayLength_TeamObjects( void *pStruct, int objectID, int currentArrayLength )
{
	C_TFTeam *pPlayer = (C_TFTeam*)pStruct;

	if ( pPlayer->m_aObjects.Count() != currentArrayLength )
	{
		pPlayer->m_aObjects.SetSize( currentArrayLength );
	}
}

//-----------------------------------------------------------------------------
// Purpose: RecvProxy that converts the Player's NPC UtlVector to entindexes
//-----------------------------------------------------------------------------
void RecvProxy_TeamNPCList( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	C_TFTeam *pPlayer = (C_TFTeam*)pStruct;
	CBaseHandle *pHandle = (CBaseHandle*)(&(pPlayer->m_aNPCs[pData->m_iElement])); 
	RecvProxy_IntToEHandle( pData, pStruct, pHandle );
}

void RecvProxyArrayLength_TeamNPCs( void *pStruct, int NPCID, int currentArrayLength )
{
	C_TFTeam *pPlayer = (C_TFTeam*)pStruct;

	if ( pPlayer->m_aNPCs.Count() != currentArrayLength )
	{
		pPlayer->m_aNPCs.SetSize( currentArrayLength );
	}
}

IMPLEMENT_CLIENTCLASS_DT( C_TFTeam, DT_TFTeam, CTFTeam )

	RecvPropInt( RECVINFO( m_nFlagCaptures ) ),
	RecvPropInt( RECVINFO( m_iRole ) ),

	RecvPropArray2( 
	RecvProxyArrayLength_TeamObjects,
	RecvPropInt( "team_object_array_element", 0, SIZEOF_IGNORE, 0, RecvProxy_TeamObjectList ), 
	MAX_PLAYERS * MAX_OBJECTS_PER_PLAYER, 
	0, 
	"team_object_array"	),

	RecvPropArray2( 
	RecvProxyArrayLength_TeamNPCs,
	RecvPropInt( "team_npc_array_element", 0, SIZEOF_IGNORE, 0, RecvProxy_TeamNPCList ), 
	1024, 
	0, 
	"team_npc_array"	),

END_RECV_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TFTeam::C_TFTeam()
{
	m_nFlagCaptures = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TFTeam::~C_TFTeam()
{
}

//-----------------------------------------------------------------------------
// Purpose: Get the localized name for the team
//-----------------------------------------------------------------------------
char* C_TFTeam::Get_Name( void )
{
	if ( Q_stricmp( m_szTeamname, "blue" ) == 0 )
	{
		return "BLU";
	}
	else if ( Q_stricmp( m_szTeamname, "red" ) == 0 )
	{
		return "RED";
	}
	else if (Q_stricmp(m_szTeamname, "green") == 0)
	{
		return "GRN";
	}
	else if (Q_stricmp(m_szTeamname, "yellow") == 0)
	{
		return "YLW";
	}

	return m_szTeamname;
}

//-----------------------------------------------------------------------------
// Purpose: Get the C_TFTeam for the specified team number
//-----------------------------------------------------------------------------
C_TFTeam *GetGlobalTFTeam( int iTeamNumber )
{
	for ( int i = 0; i < g_Teams.Count(); i++ )
	{
		if ( g_Teams[i]->GetTeamNumber() == iTeamNumber )
			return ( dynamic_cast< C_TFTeam* >( g_Teams[i] ) );
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int C_TFTeam::GetNumObjects( int iObjectType )
{
	// Asking for a count of a specific object type?
	if ( iObjectType > 0 )
	{
		int iCount = 0;
		for ( int i = 0; i < GetNumObjects(); i++ )
		{
			CBaseObject *pObject = GetObject(i);
			if ( pObject && pObject->GetType() == iObjectType )
			{
				iCount++;
			}
		}
		return iCount;
	}

	return m_aObjects.Count();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseObject *C_TFTeam::GetObject( int num )
{
	Assert( num >= 0 && num < m_aObjects.Count() );
	return m_aObjects[ num ];
}

//-----------------------------------------------------------------------------
// Gets the ith NPC on the team (may return NULL) 
//-----------------------------------------------------------------------------
C_AI_BaseNPC* C_TFTeam::GetNPC( int num )
{
	Assert( num >= 0 && num < m_aNPCs.Count() );
	return m_aNPCs[ num ];
}

int C_TFTeam::GetNumNPCs( void )
{
	return m_aNPCs.Count();
}