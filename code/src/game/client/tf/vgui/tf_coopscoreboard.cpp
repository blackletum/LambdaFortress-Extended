//========= Copyright � 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#include <tier1/fmtstr.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/RichText.h>
#include <vgui_controls/Frame.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/SectionedListPanel.h>
#include <vgui_controls/ImageList.h>
#include <game/client/iviewport.h>
#include <KeyValues.h>
#include <filesystem.h>
#include "IGameUIFuncs.h" // for key bindings

#include "tf_controls.h"
#include "tf_shareddefs.h"
#include "tf_coopscoreboard.h"
#include "tf_gamestats_shared.h"
#include "tf_hud_statpanel.h"
#include "c_playerresource.h"
#include "c_tf_playerresource.h"
#include "c_tf_team.h"
#include "c_tf_player.h"
#include "vgui_avatarimage.h"
#include "tf_gamerules.h"

#if defined ( _X360 )
#include "engine/imatchmaking.h"
#endif

using namespace vgui;

#define SCOREBOARD_MAX_LIST_ENTRIES 12

extern bool IsInCommentaryMode( void );
extern const char *GetMapDisplayName( const char *mapName );

vgui::IImage* GetDefaultAvatarImage( C_BasePlayer *pPlayer );

extern ConVar tf_scoreboard_alt_class_icons;
extern ConVar tf_scoreboard_mouse_mode;
extern ConVar tf_scoreboard_ping_as_text;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFCoOpScoreBoardDialog::CTFCoOpScoreBoardDialog( IViewPort *pViewPort ) : CClientScoreBoardDialog( pViewPort )
{
	SetProportional(true);
	SetKeyBoardInputEnabled(false);
	SetMouseInputEnabled(false);
	SetScheme( "ClientScheme" );

	m_pPlayerListBlue = new SectionedListPanel( this, "BluePlayerList" );
	m_pPlayerListRed = new SectionedListPanel( this, "RedPlayerList" );
	m_pLabelPlayerName = new CExLabel(this, "PlayerNameLabel", "");
	m_pLabelMapName = new CExLabel(this, "MapName", "");
	m_pImagePanelHorizLine = new ImagePanel( this, "HorizontalLine" );
	m_pClassImage = new CTFClassImage( this, "ClassImage" );
	m_pLocalPlayerStatsPanel = new EditablePanel( this, "LocalPlayerStatsPanel" );
	m_pLocalPlayerDuelStatsPanel = new EditablePanel(this, "LocalPlayerDuelStatsPanel");
	m_pSpectatorsInQueue = new CExLabel(this, "SpectatorsInQueue", "");
	m_pServerTimeLeftValue = new CExLabel( this, "ServerTimeLeftValue", "" );
	m_iImageDead = 0;
	m_iImageDominated = 0;
	m_iImageNemesis = 0;
	m_iImagePing = 0;
	m_iImagePingBot = 0;

	ListenForGameEvent( "server_spawn" );

	SetDialogVariable( "server", "" );

	SetVisible( false );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFCoOpScoreBoardDialog::~CTFCoOpScoreBoardDialog()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::PerformLayout()
{
	BaseClass::PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings("Resource/UI/scoreboard_coop.res");

	KeyValues *pConditions = NULL;

	if ( TFGameRules() && TFGameRules()->IsBluCoOp() )
	{
		pConditions = new KeyValues( "conditions" );
		AddSubKeyNamed( pConditions, "if_team_blue" );
	}

	if ( m_pImageList )
	{
		m_iImageDead = m_pImageList->AddImage( scheme()->GetImage( "../hud/leaderboard_dead", true ) );
		m_iImageDominated = m_pImageList->AddImage( scheme()->GetImage( "../hud/leaderboard_dominated", true ) );
		m_iImageNemesis = m_pImageList->AddImage( scheme()->GetImage( "../hud/leaderboard_nemesis", true ) );

		// resize the images to our resolution
		for ( int i = 1; i < m_pImageList->GetImageCount(); i++ )
		{
			int wide = 13, tall = 13;

			if ( m_pImageList->GetImage( i ) )
			{
				m_pImageList->GetImage(i)->SetSize(scheme()->GetProportionalScaledValueEx( GetScheme(), wide ), scheme()->GetProportionalScaledValueEx( GetScheme(),tall ) );
			}
		}
	}

	SetPlayerListImages( m_pPlayerListBlue );
	SetPlayerListImages( m_pPlayerListRed );

	SetBgColor( Color( 0, 0, 0, 0) );
	SetBorder( NULL );
	SetVisible( false );

	m_hTimeLeftFont = pScheme->GetFont( "ScoreboardMedium", true );
	m_hTimeLeftNotSetFont = pScheme->GetFont( "ScoreboardVerySmall", true );

	// This is the panel used by Arena to show people who are waiting to play.
	// Since we don't have Arena yet, disable it fo rnow
	if ( m_pSpectatorsInQueue) 
		m_pSpectatorsInQueue->SetVisible( false );

	Reset();

	if ( pConditions )
		pConditions->deleteThis();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::ShowPanel(bool bShow)
{
	// Catch the case where we call ShowPanel before ApplySchemeSettings, eg when
	// going from windowed <-> fullscreen
	if ( m_pImageList == NULL )
	{
		InvalidateLayout( true, true );
	}

	// Don't show in commentary mode
	if ( IsInCommentaryMode() )
	{
		bShow = false;
	}
	
	if ( IsVisible() == bShow )
	{
		return;
	}

	int iRenderGroup = gHUD.LookupRenderGroupIndexByName( "global" );

	if ( bShow )
	{
		if ( TFGameRules() && TFGameRules()->IsCoOp() || TFGameRules()->IsZombieSurvival() )
		{
			m_pPlayerListBlue->SetVisible( false );
		}
		else if ( TFGameRules() && TFGameRules()->IsBluCoOp() )
		{
			m_pPlayerListRed->SetVisible( false );
		}

		SetVisible(true);
		MoveToFront();

		gHUD.LockRenderGroup(iRenderGroup);

		// Clear the selected item, this forces the default to the local player
		SectionedListPanel *pList = GetSelectedPlayerList();
		if (pList)
		{
			pList->ClearSelection();
		}
	}
	else
	{
		SetVisible( false );

		gHUD.UnlockRenderGroup( iRenderGroup );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Resets the scoreboard panel
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::Reset()
{
	InitPlayerList( m_pPlayerListBlue );
	InitPlayerList( m_pPlayerListRed );

	if ( m_pServerTimeLeftValue )
		m_pServerTimeLeftValue->SetFgColor( Color( 255, 255, 0, 255 ) );
}

//-----------------------------------------------------------------------------
// Purpose: Inits the player list in a list panel
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::InitPlayerList(SectionedListPanel *pPlayerList)
{
	pPlayerList->SetVerticalScrollbar( false );
	pPlayerList->RemoveAll();
	pPlayerList->RemoveAllSections();
	pPlayerList->AddSection( 0, "Players", TFPlayerSortFunc );
	pPlayerList->SetSectionAlwaysVisible( 0, true );
	pPlayerList->SetSectionFgColor( 0, Color( 255, 255, 255, 255 ) );
	pPlayerList->SetBgColor( Color( 0, 0, 0, 0 ) );
	pPlayerList->SetBorder( NULL );

	// Avatars are always displayed at 32x32 regardless of resolution
	if ( ShowAvatars() )
	{
		pPlayerList->AddColumnToSection( 0, "avatar", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iAvatarWidth );
	}

	pPlayerList->AddColumnToSection( 0, "name", "#TF_Scoreboard_Name", 0, m_iNameWidth );	
	pPlayerList->AddColumnToSection( 0, "dominating", "", SectionedListPanel::COLUMN_IMAGE, m_iNemesisWidth );
	//pPlayerList->AddColumnToSection( 0, "status", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iStatusWidth );
	pPlayerList->AddColumnToSection( 0, "nemesis", "", SectionedListPanel::COLUMN_IMAGE, m_iNemesisWidth );
	pPlayerList->AddColumnToSection( 0, "score", "#TF_Scoreboard_Score", SectionedListPanel::COLUMN_RIGHT, m_iScoreWidth );
	pPlayerList->AddColumnToSection( 0, "class", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_RIGHT, m_iClassWidth );

	if ( tf_scoreboard_ping_as_text.GetBool() )
		pPlayerList->AddColumnToSection( 0, "ping", "#TF_Scoreboard_Ping", SectionedListPanel::COLUMN_RIGHT, m_iPingWidth );
	else
		pPlayerList->AddColumnToSection( 0, "ping", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_RIGHT, m_iPingWidth );
}

//-----------------------------------------------------------------------------
// Purpose: Builds the image list to use in the player list
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::SetPlayerListImages(vgui::SectionedListPanel *pPlayerList)
{
	pPlayerList->SetImageList( m_pImageList, false );
	pPlayerList->SetVisible( true );
}

//-----------------------------------------------------------------------------
// Purpose: Updates the dialog
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::Update()
{
	UpdateTeamInfo();
	UpdatePlayerList();
	UpdateSpectatorList();
	UpdatePlayerDetails();
	MoveToCenterOfScreen();


	if ( TFGameRules() )
	{
		if ( mp_timelimit.GetInt() > 0 && TFGameRules()->GetTimeLeft() > 0 )
		{
			m_pServerTimeLeftValue->SetFont( m_hTimeLeftFont );
			int iTimeLeft = TFGameRules()->GetTimeLeft();

			char szHours[5];
			char szMinutes[3];
			char szSeconds[3];

			wchar_t wszHours[5] = L"";
			wchar_t wszMinutes[3] = L"";
			wchar_t wszSeconds[3] = L"";

			if ( iTimeLeft <= 0 )
			{
				Q_snprintf(szHours, sizeof(szHours), "0");
				Q_snprintf(szMinutes, sizeof(szMinutes), "0");
				Q_snprintf(szSeconds, sizeof(szSeconds), "00");
			}
			else
			{
				Q_snprintf(szHours,   sizeof(szHours), "%d", iTimeLeft / 3600);
				Q_snprintf(szMinutes, sizeof(szMinutes), "%d", iTimeLeft / 60);
				Q_snprintf(szSeconds, sizeof(szSeconds), "%02d", iTimeLeft % 60);
			}

			g_pVGuiLocalize->ConvertANSIToUnicode( szHours, wszHours, sizeof( wszHours ) );
			g_pVGuiLocalize->ConvertANSIToUnicode( szMinutes, wszMinutes, sizeof( wszMinutes ) );
			g_pVGuiLocalize->ConvertANSIToUnicode( szSeconds, wszSeconds, sizeof( wszSeconds ) );

			wchar_t wzTimeLabelOld[256] = L"";
			wchar_t wzTimeLabelNew[256] = L"";

			if ( iTimeLeft / 3600 > 0 )
			{
				g_pVGuiLocalize->ConstructString( wzTimeLabelOld, sizeof( wzTimeLabelOld ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeft" ), 3, wszHours, wszMinutes, wszSeconds );
				g_pVGuiLocalize->ConstructString( wzTimeLabelNew, sizeof( wzTimeLabelNew ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeftNew" ), 3, wszHours, wszMinutes, wszSeconds );
			}
			else 
			{
				g_pVGuiLocalize->ConstructString( wzTimeLabelOld, sizeof( wzTimeLabelOld ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeftNoHours" ), 2, wszMinutes, wszSeconds );
				g_pVGuiLocalize->ConstructString( wzTimeLabelNew, sizeof( wzTimeLabelNew ), g_pVGuiLocalize->Find( "#Scoreboard_TimeLeftNoHoursNew" ), 2, wszMinutes, wszSeconds );
			}

			SetDialogVariable( "servertimeleft", wzTimeLabelOld );
			SetDialogVariable( "servertime", wzTimeLabelNew );
		}
		else if ( mp_timelimit.GetInt() > 0 ) // Timer is set and has run out
		{
			m_pServerTimeLeftValue->SetFont( m_hTimeLeftNotSetFont );
			SetDialogVariable( "servertimeleft", g_pVGuiLocalize->Find( "#Scoreboard_ChangeOnRoundEnd" ) );
			SetDialogVariable( "servertime", g_pVGuiLocalize->Find( "#Scoreboard_ChangeOnRoundEndNew" ) );
		}
		else
		{
			m_pServerTimeLeftValue->SetFont( m_hTimeLeftNotSetFont );
			SetDialogVariable( "servertimeleft", g_pVGuiLocalize->Find( "#Scoreboard_NoTimeLimit" ) );
			SetDialogVariable( "servertime", g_pVGuiLocalize->Find( "#Scoreboard_NoTimeLimitNew" ) );
		}
	}

	char szMapName[MAX_MAP_NAME];
	Q_FileBase( engine->GetLevelName(), szMapName, sizeof( szMapName ) );
	Q_strlower( szMapName );
	SetDialogVariable( "mapname", GetMapDisplayName( szMapName ) );

	// update every second
	m_fNextUpdateTime = gpGlobals->curtime + 1.0f; 
}

//-----------------------------------------------------------------------------
// Purpose: Updates information about teams
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::UpdateTeamInfo()
{
	// update the team sections in the scoreboard
	for ( int teamIndex = TF_TEAM_RED; teamIndex <= TF_TEAM_BLUE; teamIndex++ )
	{
		wchar_t *teamName = NULL;
		C_Team *team = GetGlobalTeam( teamIndex );
		if ( team )
		{
			// choose dialog variables to set depending on team
			const char *pDialogVarTeamScore = NULL;
			const char *pDialogVarTeamPlayerCount = NULL;
			const char *pDialogVarTeamName = NULL;
			switch ( teamIndex ) {
				case TF_TEAM_RED:
					pDialogVarTeamScore = "redteamscore";
					pDialogVarTeamPlayerCount = "redteamplayercount";
					pDialogVarTeamName = "redteamname";
					break;
				case TF_TEAM_BLUE:
					pDialogVarTeamScore = "blueteamscore";
					pDialogVarTeamPlayerCount = "blueteamplayercount";
					pDialogVarTeamName = "blueteamname";
					break;
				default:
					Assert( false );
					break;
			}

			// update # of players on each team
			wchar_t name[64];
			wchar_t string1[1024];
			wchar_t wNumPlayers[6];
			_snwprintf( wNumPlayers, ARRAYSIZE( wNumPlayers ), L"%i", team->Get_Number_Players() );
			if ( !teamName && team )
			{
				g_pVGuiLocalize->ConvertANSIToUnicode( team->Get_Name(), name, sizeof( name ) );
				teamName = name;
			}
			if ( team->Get_Number_Players() == 1 )
			{
				g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#TF_ScoreBoard_Player" ), 1, wNumPlayers );
			}
			else
			{
				g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#TF_ScoreBoard_Players" ), 1, wNumPlayers );
			}

			// set # of players for team in dialog
			SetDialogVariable( pDialogVarTeamPlayerCount, string1 );

			// set team score in dialog
			SetDialogVariable( pDialogVarTeamScore, team->Get_Score() );	

			// set team name
			SetDialogVariable( pDialogVarTeamName, team->Get_Name() );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the player list
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::UpdatePlayerList()
{
	Reset();

	int iSelectedPlayerIndex = GetLocalPlayerIndex();

	// Save off which player we had selected
	SectionedListPanel *pList = GetSelectedPlayerList();

	if ( pList )
	{
		int itemID = pList->GetSelectedItem();

		if ( itemID >= 0 )
		{
			KeyValues *pInfo = pList->GetItemData( itemID );
			if ( pInfo )
			{
				iSelectedPlayerIndex = pInfo->GetInt( "playerIndex" );
			}
		}
	}	

	m_pPlayerListRed->RemoveAll();
	m_pPlayerListBlue->RemoveAll();

	C_TF_PlayerResource *tf_PR = dynamic_cast<C_TF_PlayerResource *>( g_PR );
	if ( !tf_PR )
		return;
	C_TFPlayer *pLocalPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pLocalPlayer )
		return;

	int localteam = pLocalPlayer->GetTeamNumber();

	bool bMadeSelection = false;

	for( int playerIndex = 1 ; playerIndex <= MAX_PLAYERS; playerIndex++ )
	{
		if( g_PR->IsConnected( playerIndex ) )
		{
			SectionedListPanel *pPlayerList = NULL;
			switch ( g_PR->GetTeam( playerIndex ) )
			{
			case TF_TEAM_BLUE:
				pPlayerList = m_pPlayerListBlue;
				break;
			case TF_TEAM_RED:
				pPlayerList = m_pPlayerListRed;
				break;
			}
			if ( null == pPlayerList )
				continue;			

			const char *szName = tf_PR->GetPlayerName( playerIndex );
			int score = tf_PR->GetTotalScore( playerIndex );

			KeyValues *pKeyValues = new KeyValues( "data" );

			pKeyValues->SetInt( "playerIndex", playerIndex );
			pKeyValues->SetString( "name", szName );
			pKeyValues->SetInt( "score", score );

			// can only see class information if we're on the same team
			if ( !AreEnemyTeams( g_PR->GetTeam( playerIndex ), localteam ) && !( localteam == TEAM_UNASSIGNED ) )
			{
				// class name
				if( g_PR->IsConnected( playerIndex ) )
				{
					int iClass = tf_PR->GetPlayerClass( playerIndex );
					if ( GetLocalPlayerIndex() == playerIndex && !tf_PR->IsAlive( playerIndex ) ) 
					{
						// If this is local player and he is dead, show desired class (which he will spawn as) rather than current class.
						C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
						int iDesiredClass = pPlayer->m_Shared.GetDesiredPlayerClassIndex();
						// use desired class unless it's random -- if random, his future class is not decided until moment of spawn
						if ( TF_CLASS_RANDOM != iDesiredClass )
						{
							iClass = iDesiredClass;
						}
					} 
					else 
					{
						// for non-local players, show the current class
						iClass = tf_PR->GetPlayerClass( playerIndex );
					}

					if( iClass >= TF_FIRST_NORMAL_CLASS )
					{
						//pKeyValues->SetString( "class", g_aPlayerClassNames[iClass] );
						pKeyValues->SetInt( "class", tf_PR->IsAlive( playerIndex) ? m_iClassEmblem[iClass] : m_iClassEmblemDead[iClass] );
					}
					else
					{
						pKeyValues->SetString( "class", "" );
					}
				}				
			}
			else
			{
				C_TFPlayer *pPlayerOther = ToTFPlayer( UTIL_PlayerByIndex( playerIndex ) );

				bool bUseTruncatedNames = false;

				if ( pPlayerOther && pPlayerOther->m_Shared.IsPlayerDominated( pLocalPlayer->entindex() ) )
				{
					// if local player is dominated by this player, show a nemesis icon
					pKeyValues->SetInt( "nemesis", m_iImageNemesis );
					pKeyValues->SetString( "class", bUseTruncatedNames ? "#TF_Nemesis_lodef" : "#TF_Nemesis" );
				}
				else if ( pLocalPlayer->m_Shared.IsPlayerDominated( playerIndex) )
				{
					// if this player is dominated by the local player, show the domination icon
					pKeyValues->SetInt( "nemesis", m_iImageDominated );
					pKeyValues->SetString( "class", bUseTruncatedNames ? "#TF_Dominated_lodef" : "#TF_Dominated" );
				}
			}

			// display whether player is alive or dead (all players see this for all other players on both teams)
			//pKeyValues->SetInt( "status", tf_PR->IsAlive( playerIndex ) ?  0 : m_iImageDead );

			if ( g_PR->GetPing( playerIndex ) < 1 )
			{
				if ( g_PR->IsFakePlayer( playerIndex ) )
				{
					if ( tf_scoreboard_ping_as_text.GetBool() )
					{
						pKeyValues->SetString( "ping", "#TF_Scoreboard_Bot" );
					}
					else
					{
						switch ( g_PR->GetTeam( playerIndex ) )
						{
						case TF_TEAM_BLUE:
							m_iImagePingBot = m_pImageList->AddImage( scheme()->GetImage( tf_PR->IsAlive( playerIndex ) ? "../hud/scoreboard_ping_bot_blue" : "../hud/scoreboard_ping_bot_blue_d", true ) );
							break;
						case TF_TEAM_RED:
							m_iImagePingBot = m_pImageList->AddImage( scheme()->GetImage( tf_PR->IsAlive( playerIndex ) ? "../hud/scoreboard_ping_bot_red" : "../hud/scoreboard_ping_bot_red_d", true ) );
							break;
						}

						pKeyValues->SetInt( "ping", m_iImagePingBot );

						int wide = 13, tall = 13;
						if ( m_pImageList->GetImage( m_iImagePingBot ) )
						{
							m_pImageList->GetImage( m_iImagePingBot )->SetSize(scheme()->GetProportionalScaledValueEx( GetScheme(), wide ), scheme()->GetProportionalScaledValueEx( GetScheme(),tall ) );
						}

					}
				}
				else
				{
					pKeyValues->SetString( "ping", "" );
				}
			}
			else
			{
				if ( tf_scoreboard_ping_as_text.GetBool() )
				{
					pKeyValues->SetInt( "ping", g_PR->GetPing( playerIndex ) );
				}
				else
				{
					pKeyValues->SetInt( "ping", m_iImagePing );

					int wide = 13, tall = 13;
					if ( m_pImageList->GetImage( m_iImagePing ) )
					{
						m_pImageList->GetImage( m_iImagePing )->SetSize(scheme()->GetProportionalScaledValueEx( GetScheme(), wide ), scheme()->GetProportionalScaledValueEx( GetScheme(),tall ) );
					}

					// TODO: fix math
					if ( g_PR->GetPing( playerIndex ) < 100 )
						m_iImagePing = m_pImageList->AddImage( scheme()->GetImage( tf_PR->IsAlive( playerIndex ) ? "../hud/scoreboard_ping_low" : "../hud/scoreboard_ping_low_d", true ) );
					else if ( g_PR->GetPing( playerIndex ) < 200 )
						m_iImagePing = m_pImageList->AddImage( scheme()->GetImage( tf_PR->IsAlive( playerIndex ) ? "../hud/scoreboard_ping_med" : "../hud/scoreboard_ping_med_d", true ) );
					else if ( g_PR->GetPing( playerIndex ) < 300 )
						m_iImagePing = m_pImageList->AddImage( scheme()->GetImage( tf_PR->IsAlive( playerIndex ) ? "../hud/scoreboard_ping_high" : "../hud/scoreboard_ping_high_d", true ) );
					else if ( g_PR->GetPing( playerIndex ) > 400 )
						m_iImagePing = m_pImageList->AddImage( scheme()->GetImage( tf_PR->IsAlive( playerIndex ) ? "../hud/scoreboard_ping_very_high" : "../hud/scoreboard_ping_very_high_d", true ) );
				}
			}

			for ( int i = 0; i < TF_CLASS_COUNT_ALL; i++ )
			{
				if ( tf_scoreboard_alt_class_icons.GetBool() )
				{
					m_iClassEmblem[i] = m_pImageList->AddImage( scheme()->GetImage( g_aPlayerClassEmblemsAlt[i - 1], true ) );
					m_iClassEmblemDead[i] = m_pImageList->AddImage( scheme()->GetImage( g_aPlayerClassEmblemsAltDead[i - 1], true ) );
				}
				else
				{
					m_iClassEmblem[i] = m_pImageList->AddImage( scheme()->GetImage( g_aPlayerClassEmblems[i - 1], true ) );
					m_iClassEmblemDead[i] = m_pImageList->AddImage( scheme()->GetImage( g_aPlayerClassEmblemsDead[i - 1], true ) );
				}

				int wide = 13, tall = 13;
				if ( m_pImageList->GetImage( m_iClassEmblem[i] ) )
				{
					m_pImageList->GetImage( m_iClassEmblem[i] )->SetSize(scheme()->GetProportionalScaledValueEx( GetScheme(), wide ), scheme()->GetProportionalScaledValueEx( GetScheme(),tall ) );
				}

				if ( m_pImageList->GetImage( m_iClassEmblemDead[i] ) )
				{
					m_pImageList->GetImage( m_iClassEmblemDead[i] )->SetSize(scheme()->GetProportionalScaledValueEx( GetScheme(), wide ), scheme()->GetProportionalScaledValueEx( GetScheme(),tall ) );
				}
			}

			UpdatePlayerAvatar( playerIndex, pKeyValues );
			
			int itemID = pPlayerList->AddItem( 0, pKeyValues );
			Color clr = g_PR->GetTeamColor( g_PR->GetTeam( playerIndex ) );
			if ( !tf_PR->IsAlive( playerIndex ) )
				clr.SetColor( clr.r(), clr.g(), clr.b(), clr.a() / 2 );

			pPlayerList->SetItemFgColor( itemID, clr );

			if ( iSelectedPlayerIndex == playerIndex )
			{
				bMadeSelection = true;
				pPlayerList->SetSelectedItem( itemID );
			}
			else
			{
				pPlayerList->SetItemBgColor( itemID, Color( 0, 0, 0, 64 ) );
			}

			pKeyValues->deleteThis();
		}
	}

	// If we're on spectator, find a default selection
	if ( !bMadeSelection )
	{
		if ( m_pPlayerListBlue->GetItemCount() > 0 )
		{
			m_pPlayerListBlue->SetSelectedItem( 0 );
		}
		else if ( m_pPlayerListRed->GetItemCount() > 0 )
		{
			m_pPlayerListRed->SetSelectedItem( 0 );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates the spectator list
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::UpdateSpectatorList()
{
	// Spectators
	char szSpectatorList[512] = "" ;
	int nSpectators = 0;
	for( int playerIndex = 1 ; playerIndex <= MAX_PLAYERS; playerIndex++ )
	{
		if ( ShouldShowAsSpectator( playerIndex ) )
		{
			if ( nSpectators > 0 )
			{
				Q_strncat( szSpectatorList, ", ", ARRAYSIZE( szSpectatorList ) );
			}

			Q_strncat( szSpectatorList, g_PR->GetPlayerName( playerIndex ), ARRAYSIZE( szSpectatorList ) );
			nSpectators++;
		}
	}

	wchar_t wzSpectators[512] = L"";
	if ( nSpectators > 0 )
	{
		const char *pchFormat = ( 1 == nSpectators ? "#ScoreBoard_Spectator" : "#ScoreBoard_Spectators" );

		wchar_t wzSpectatorCount[16];
		wchar_t wzSpectatorList[1024];
		_snwprintf( wzSpectatorCount, ARRAYSIZE( wzSpectatorCount ), L"%i", nSpectators );
		g_pVGuiLocalize->ConvertANSIToUnicode( szSpectatorList, wzSpectatorList, sizeof( wzSpectatorList ) );
		g_pVGuiLocalize->ConstructString( wzSpectators, sizeof(wzSpectators), g_pVGuiLocalize->Find( pchFormat), 2, wzSpectatorCount, wzSpectatorList );
	}
	SetDialogVariable( "spectators", wzSpectators );
}

//-----------------------------------------------------------------------------
// Purpose: Updates details about a player
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::UpdatePlayerDetails()
{
	ClearPlayerDetails();

	C_TF_PlayerResource *tf_PR = dynamic_cast<C_TF_PlayerResource *>( g_PR );
	if ( !tf_PR )
		return;
	
	C_TFPlayer *pLocalPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( !pLocalPlayer )
		return;

	int playerIndex = pLocalPlayer->entindex();

	// Make sure the selected player is still connected. 
	if ( !tf_PR->IsConnected( playerIndex ) ) 
		return;

	if ( engine->IsHLTV() )
	{
		SetDialogVariable( "playername", tf_PR->GetPlayerName( playerIndex ) );
		return;
	}

	RoundStats_t &roundStats = GetStatPanel()->GetRoundStatsCurrentGame();

	if ( m_pLocalPlayerStatsPanel )
	{
		m_pLocalPlayerStatsPanel->SetDialogVariable( "kills", tf_PR->GetPlayerScore( playerIndex ) );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "deaths", tf_PR->GetDeaths( playerIndex ) );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "assists", roundStats.m_iStat[TFSTAT_KILLASSISTS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "destruction", roundStats.m_iStat[TFSTAT_BUILDINGSDESTROYED] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "captures", roundStats.m_iStat[TFSTAT_CAPTURES] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "defenses", roundStats.m_iStat[TFSTAT_DEFENSES] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "dominations", roundStats.m_iStat[TFSTAT_DOMINATIONS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "revenge", roundStats.m_iStat[TFSTAT_REVENGE] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "healing", roundStats.m_iStat[TFSTAT_HEALING] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "invulns", roundStats.m_iStat[TFSTAT_INVULNS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "teleports", roundStats.m_iStat[TFSTAT_TELEPORTS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "headshots", roundStats.m_iStat[TFSTAT_HEADSHOTS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "backstabs", roundStats.m_iStat[TFSTAT_BACKSTABS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "bonus", roundStats.m_iStat[TFSTAT_BONUS] );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "support", 0 );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "damage", roundStats.m_iStat[TFSTAT_DAMAGE] );

	}
	SetDialogVariable( "playername", tf_PR->GetPlayerName( playerIndex ) );
	SetDialogVariable( "playerscore", GetPointsString( tf_PR->GetTotalScore( playerIndex ) ) );

	Color clr = g_PR->GetTeamColor( g_PR->GetTeam( playerIndex ) );
	m_pLabelPlayerName->SetFgColor( clr );
	m_pImagePanelHorizLine->SetFillColor( clr );

	int iClass = pLocalPlayer->m_Shared.GetDesiredPlayerClassIndex();
	int iTeam = pLocalPlayer->GetTeamNumber();
	if ( ( iTeam >= FIRST_GAME_TEAM ) && ( iClass >= TF_FIRST_NORMAL_CLASS ) && ( iClass < TF_CLASS_COUNT_ALL ) )
	{
		m_pClassImage->SetClass( iTeam, iClass, 0 );
		m_pClassImage->SetVisible( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Clears score details
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::ClearPlayerDetails()
{
	m_pClassImage->SetVisible( false );

	// HLTV has no game stats
	bool bVisible = !engine->IsHLTV();

	if (m_pLocalPlayerStatsPanel)
	{
		m_pLocalPlayerStatsPanel->SetDialogVariable( "kills", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "deaths", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "assists", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "destruction", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "captures", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "defenses", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "dominations", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "revenge", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "invulns", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "headshots", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "teleports", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "healing", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "backstabs", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "bonus", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "support", "" );
		m_pLocalPlayerStatsPanel->SetDialogVariable( "damage", "" );

		m_pLocalPlayerStatsPanel->SetControlVisible( "KillsLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "DeathsLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "AssistsLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "DestructionLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "CapturesLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "DefensesLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "DominationLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "RevengeLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "InvulnLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "HeadshotsLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "TeleportsLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "HealingLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "BackstabsLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "BonusLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "SupportLabel", bVisible );
		m_pLocalPlayerStatsPanel->SetControlVisible( "DamageLabel", bVisible );
	}

	SetDialogVariable("playername", "");
	SetDialogVariable("playerscore", "");
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting players
//-----------------------------------------------------------------------------
bool CTFCoOpScoreBoardDialog::TFPlayerSortFunc(vgui::SectionedListPanel *list, int itemID1, int itemID2)
{
	KeyValues *it1 = list->GetItemData(itemID1);
	KeyValues *it2 = list->GetItemData(itemID2);
	Assert(it1 && it2);

	// first compare score
	int v1 = it1->GetInt("score");
	int v2 = it2->GetInt("score");
	if (v1 > v2)
		return true;
	else if (v1 < v2)
		return false;

	// if score is the same, use player index to get deterministic sort
	int iPlayerIndex1 = it1->GetInt( "playerIndex" );
	int iPlayerIndex2 = it2->GetInt( "playerIndex" );
	return ( iPlayerIndex1 > iPlayerIndex2 );
}

//-----------------------------------------------------------------------------
// Purpose: Returns whether the specified player index is a spectator
//-----------------------------------------------------------------------------
bool CTFCoOpScoreBoardDialog::ShouldShowAsSpectator( int iPlayerIndex )
{
	C_TF_PlayerResource *tf_PR = dynamic_cast<C_TF_PlayerResource *>( g_PR );
	if ( !tf_PR )
		return false;

	// see if player is connected
	if ( tf_PR->IsConnected( iPlayerIndex ) ) 
	{
		// either spectating or unassigned team should show in spectator list
		int iTeam = tf_PR->GetTeam( iPlayerIndex );
		if ( TEAM_SPECTATOR == iTeam )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Returns whether the specified player is waiting to play/unassigned
//-----------------------------------------------------------------------------
bool CTFCoOpScoreBoardDialog::ShouldShowAsUnassigned( int iPlayerIndex )
{
	C_TF_PlayerResource *tf_PR = dynamic_cast<C_TF_PlayerResource *>(g_PR);
	if (!tf_PR)
		return false;

	// If the player hasn't connected yet, show him in the status bar
	if (!tf_PR->IsConnected(iPlayerIndex))
		return true;

	// If the player has connected, but hasn't picked anything.
	int iTeam = tf_PR->GetTeam(iPlayerIndex);
	if (TEAM_UNASSIGNED == iTeam)
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Event handler
//-----------------------------------------------------------------------------
void CTFCoOpScoreBoardDialog::FireGameEvent(IGameEvent *event)
{
	const char *type = event->GetName();

	if ( 0 == Q_strcmp( type, "server_spawn" ) )
	{		
		// set server name in scoreboard
		const char *hostname = event->GetString( "hostname" );
		wchar_t wzHostName[256];
		wchar_t wzServerLabel[256];
		g_pVGuiLocalize->ConvertANSIToUnicode( hostname, wzHostName, sizeof( wzHostName ) );
		g_pVGuiLocalize->ConstructString( wzServerLabel, sizeof(wzServerLabel), g_pVGuiLocalize->Find( "#Scoreboard_Server" ), 1, wzHostName );
		SetDialogVariable( "server", wzServerLabel );
	}

	if( IsVisible() )
	{
		Update();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
SectionedListPanel *CTFCoOpScoreBoardDialog::GetSelectedPlayerList( void )
{
	SectionedListPanel *pList = NULL;

	// navigation
	if ( m_pPlayerListBlue->GetSelectedItem() >= 0 )
	{
		pList = m_pPlayerListBlue;
	}
	else if ( m_pPlayerListRed->GetSelectedItem() >= 0 )
	{
		pList = m_pPlayerListRed;
	}

	return pList;
}