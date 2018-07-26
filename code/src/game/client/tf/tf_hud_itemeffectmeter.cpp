//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "c_tf_player.h"
#include "iclientmode.h"
#include "ienginevgui.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ProgressBar.h>
#include "engine/IEngineSound.h"
#include "tf_controls.h"
#include "tf_weaponbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

class CHudItemEffects;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudItemEffectMeter : public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudItemEffectMeter, EditablePanel );

public:
	CHudItemEffectMeter( Panel *pParent, const char *pElementName );

	virtual void	ApplySchemeSettings( IScheme *scheme );
	virtual void	PerformLayout( void );
	virtual void	LevelInit( void );

	void			UpdateStatus( void );
	int				GetSlot( void ) { return m_iSlot; }
	void			SetSlot( int iSlot ) { m_iSlot = iSlot; }
	void			SetWeapon( C_TFWeaponBase *pWeapon );

private:
	ContinuousProgressBar *m_pEffectMeter;
	CExLabel *m_pEffectMeterLabel;

	int m_iSlot;
	CHandle<C_TFWeaponBase> m_hWeapon;
	float m_flOldCharge;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudItemEffectMeter::CHudItemEffectMeter( Panel *pParent, const char *pElementName ) : EditablePanel( pParent, pElementName )
{
	m_pEffectMeter = new ContinuousProgressBar( this, "ItemEffectMeter" );
	m_pEffectMeterLabel = new CExLabel( this, "ItemEffectMeterLabel", "" );
	m_iSlot = 0;
	m_flOldCharge = 1.0f;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "resource/UI/HudItemEffectMeter.res" );
}

// ---------------------------------------------------------------------------- -
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::LevelInit( void )
{
	m_hWeapon = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::PerformLayout( void )
{
	if ( m_pEffectMeterLabel && m_hWeapon.Get() )
	{
		wchar_t *pszLocalized = g_pVGuiLocalize->Find( m_hWeapon->GetEffectLabelText() );
		if ( pszLocalized )
		{
			m_pEffectMeterLabel->SetText( pszLocalized );
		}
		else
		{
			m_pEffectMeterLabel->SetText( m_hWeapon->GetEffectLabelText() );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffectMeter::UpdateStatus( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();

	if ( !pPlayer || !pPlayer->IsAlive() )
	{
		m_flOldCharge = 1.0f;
		return;
	}

	// Find a weapon in the loadout slot we're tied to.
	C_EconEntity *pEntity = pPlayer->GetEntityForLoadoutSlot( m_iSlot );
	if ( pEntity && pEntity->IsBaseCombatWeapon() )
	{
		if ( pEntity != m_hWeapon.Get() )
		{
			// Weapon changed, reset the label and progress.
			m_hWeapon = static_cast<C_TFWeaponBase *>( pEntity );
			m_flOldCharge = m_hWeapon->GetEffectBarProgress();

			InvalidateLayout();
		}
	}
	else
	{
		m_hWeapon = NULL;
	}

	if ( !m_hWeapon.Get() || !m_hWeapon->HasChargeBar() )
	{
		m_flOldCharge = 1.0f;
		if ( IsVisible() )
			SetVisible( false );

		return;
	}

	if ( !IsVisible() )
		SetVisible( true );

	if ( m_pEffectMeter )
	{
		float flCharge = m_hWeapon->GetEffectBarProgress();
		m_pEffectMeter->SetProgress( flCharge );
		
		// Play a ding when full charged.
		if ( m_flOldCharge < 1.0f && flCharge == 1.0f && !m_hWeapon->IsWeapon( TF_WEAPON_INVIS ) )
		{
			CLocalPlayerFilter filter;
			C_BaseEntity::EmitSound( filter, SOUND_FROM_LOCAL_PLAYER, "TFPlayer.Recharged" );
		}

		m_flOldCharge = flCharge;
	}

	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_TFWeaponBase *pWeapon = ( C_TFWeaponBase *)pPlayer->GetWeapon( i );
		if ( !pWeapon )
			return;

		if ( pPlayer && pWeapon->GetWeaponID() == TF_WEAPON_LUNCHBOX_DRINK || pWeapon->GetWeaponID() == TF_WEAPON_JAR_MILK )
		{
			LoadControlSettings( "resource/UI/HudItemEffectMeter_Scout.res" );
		}
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_SodaPopper.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_Cleaver.res" );
		else if ( pPlayer && pWeapon->GetWeaponID() == TF_WEAPON_LUNCHBOX )
		{
			LoadControlSettings( "resource/UI/HudItemEffectMeter_Heavy.res" );
		}
		else if ( pPlayer && pWeapon->GetWeaponID() == TF_WEAPON_JAR )
		{
			LoadControlSettings( "resource/UI/HudItemEffectMeter_Sniper.res" );
		}
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_SniperFocus.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_Demoman.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_ParticleCannon.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_Raygun.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_SpyKnife.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_Sapper.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_Spy.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_Engineer.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_Pomson.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_Pyro.res" );
		//LoadControlSettings( "resource/UI/HUDItemEffectMeter_Organs.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_Action.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_KillStreak.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_KartCharge.res" );
		//LoadControlSettings( "resource/UI/HudItemEffectMeter_PowerupBottle.res" );
		//LoadControlSettings( "resource/UI/HudRocketPack.res" );
		else
		{
			LoadControlSettings( "resource/UI/HudItemEffectMeter.res" );
		}
		InvalidateLayout();
	}
}


class CHudItemEffects : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudItemEffects, EditablePanel );

public:
	CHudItemEffects( const char *pElementName );
	~CHudItemEffects();

	virtual void PerformLayout( void );
	virtual bool ShouldDraw( void );
	virtual void OnTick( void );

private:
	CUtlVector<CHudItemEffectMeter *> m_pEffectBars;
	
	CPanelAnimationVarAliasType( int, m_iXOffset, "x_offset", "50", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iYOffset, "y_offset", "0", "proportional_int" );
};

DECLARE_HUDELEMENT( CHudItemEffects );

CHudItemEffects::CHudItemEffects( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudItemEffects" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	// Create effect bars for primary, secondary and melee slots.
	for ( int i = 0; i < TF_PLAYER_WEAPON_COUNT; i++ )
	{
		CHudItemEffectMeter *pMeter = new CHudItemEffectMeter( this, "HudItemEffectMeter" );
		pMeter->SetSlot( i );
		m_pEffectBars.AddToTail( pMeter );
	}

	SetHiddenBits( HIDEHUD_MISCSTATUS );

	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

CHudItemEffects::~CHudItemEffects()
{
	m_pEffectBars.PurgeAndDeleteElements();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CHudItemEffects::ShouldDraw( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer || !pPlayer->IsAlive() )
		return false;

	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: Sort meters by visiblity and loadout slot.
//-----------------------------------------------------------------------------
static int EffectBarsSort( CHudItemEffectMeter * const *pMeter1, CHudItemEffectMeter * const *pMeter2 )
{
	// Visible to the right.
	if ( !( *pMeter1 )->IsVisible() && ( *pMeter2 )->IsVisible() )
		return -1;

	if ( ( *pMeter1 )->IsVisible() && !( *pMeter2 )->IsVisible() )
		return 1;

	return ( ( *pMeter1 )->GetSlot() - ( *pMeter2 )->GetSlot() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffects::PerformLayout( void )
{
	m_pEffectBars.Sort( EffectBarsSort );

	// Set panel offsets based on visibility.
	int count = m_pEffectBars.Count();
	for ( int i = 0; i < count; i++ )
	{
		m_pEffectBars[i]->SetPos( m_iXOffset * i, m_iYOffset * i );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudItemEffects::OnTick( void )
{
	C_TFPlayer *pPlayer = C_TFPlayer::GetLocalTFPlayer();
	if ( !pPlayer )
		return;

	bool bUpdateLayout = false;
	for ( int i = 0; i < m_pEffectBars.Count(); i++ )
	{
		CHudItemEffectMeter *pMeter = m_pEffectBars[i];

		bool bWasVisible = pMeter->IsVisible();
		pMeter->UpdateStatus();
		bool bVisible = pMeter->IsVisible();

		if ( bVisible != bWasVisible )
		{
			bUpdateLayout = true;
		}
	}

	if ( bUpdateLayout )
	{
		InvalidateLayout( true );
	}
}
