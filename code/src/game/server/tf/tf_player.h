//========= Copyright � 1996-2005, Valve LLC, All rights reserved. ============
//
//=============================================================================
#ifndef TF_PLAYER_H
#define TF_PLAYER_H
#pragma once

#include "basemultiplayerplayer.h"
#include "server_class.h"
#include "tf_playeranimstate.h"
#include "tf_shareddefs.h"
#include "tf_player_shared.h"
#include "tf_playerclass.h"
#include "entity_tfstart.h"
#include "tf_inventory.h"
#include "tf_weapon_medigun.h"
#include "ihasattributes.h"
#include "hl_movedata.h"
#include "hl2_player.h"
#include "tf_revive.h"

class CTFPlayer;
class CTFTeam;
class CTFGoal;
class CTFGoalItem;
class CTFItem;
class CTFWeaponBuilder;
class CBaseObject;
class CTFWeaponBase;
class CIntroViewpoint;
class CLogicPlayerProxy;
class CTFReviveMarker;

//=============================================================================
//
// Player State Information
//
class CPlayerStateInfo
{
public:

	int				m_nPlayerState;
	const char		*m_pStateName;

	// Enter/Leave state.
	void ( CTFPlayer::*pfnEnterState )();	
	void ( CTFPlayer::*pfnLeaveState )();

	// Think (called every frame).
	void ( CTFPlayer::*pfnThink )();
};

struct DamagerHistory_t
{
	DamagerHistory_t()
	{
		Reset();
	}
	void Reset()
	{
		hDamager = NULL;
		flTimeDamage = 0;
	}
	EHANDLE hDamager;
	float	flTimeDamage;
};
#define MAX_DAMAGER_HISTORY 2

//=============================================================================
//
// TF Player
//
class CTFPlayer : public CBaseMultiplayerPlayer, public IHasAttributes
{
public:
	DECLARE_CLASS( CTFPlayer, CBaseMultiplayerPlayer );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CTFPlayer();
	~CTFPlayer();

	// Creation/Destruction.
	static CTFPlayer	*CreatePlayer( const char *className, edict_t *ed );
	static CTFPlayer	*Instance( int iEnt );

	virtual void		Spawn();
	virtual int			ShouldTransmit( const CCheckTransmitInfo *pInfo );
	virtual void		ForceRespawn();
	virtual CBaseEntity	*EntSelectSpawnPoint( void );
	virtual void		InitialSpawn();
	virtual void		Precache();
	virtual bool		IsReadyToPlay( void );
	virtual bool		IsReadyToSpawn( void );
	virtual bool		ShouldGainInstantSpawn( void );
	virtual void		ResetScores( void );
	virtual void		PlayerUse( void );
	void CheckTeam(void);

	void				CreateViewModel( int iViewModel = 0 );
	CBaseViewModel		*GetOffHandViewModel();
	void				SendOffHandViewModelActivity( Activity activity );

	virtual void		CheatImpulseCommands( int iImpulse );
	virtual void		PlayerRunCommand( CUserCmd *ucmd, IMoveHelper *moveHelper);

	virtual void		CommitSuicide( bool bExplode = false, bool bForce = false );

	virtual void		LeaveVehicle( const Vector &vecExitPoint, const QAngle &vecExitAngles );

	// Combats
	virtual void		TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator );
	virtual int			TakeHealth( float flHealth, int bitsDamageType );
	virtual	void		Event_KilledOther( CBaseEntity *pVictim, const CTakeDamageInfo &info );
	virtual void		Event_Killed( const CTakeDamageInfo &info );
	virtual bool		Event_Gibbed( const CTakeDamageInfo &info );
	virtual bool		BecomeRagdoll( const CTakeDamageInfo &info, const Vector &forceVector );
	virtual void		PlayerDeathThink( void );

	virtual int			OnTakeDamage( const CTakeDamageInfo &inputInfo );
	virtual int			OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void				ApplyPushFromDamage( const CTakeDamageInfo &info, Vector &vecDir );
	void				SetBlastJumpState( int iJumpType, bool bPlaySound );
	void				ClearBlastJumpState( void );
	int					GetBlastJumpFlags( void ) { return m_nBlastJumpFlags; }
	void				SetAirblastState( bool bAirblastState );
	void				ClearAirblastState( void );
	void				AddDamagerToHistory( EHANDLE hDamager );
	void				ClearDamagerHistory();
	DamagerHistory_t	&GetDamagerHistory( int i ) { return m_DamagerHistory[i]; }
	virtual void		DamageEffect(float flDamage, int fDamageType);
	virtual	bool		ShouldCollide( int collisionGroup, int contentsMask ) const;

	void				SetHealthBuffTime( float flTime )		{ m_flHealthBuffTime = flTime; }

	CTFWeaponBase		*GetActiveTFWeapon( void ) const;
	bool				IsActiveTFWeapon(int iWeaponID);

	CEconItemView		*GetLoadoutItem( int iClass, int iSlot );
	void				HandleCommand_WeaponPreset(int iSlotNum, int iPresetNum);
	void				HandleCommand_WeaponPreset(int iClass, int iSlotNum, int iPresetNum);

	CBaseEntity			*GiveNamedItem( const char *pszName, int iSubType = 0, CEconItemView* pItem = NULL );

	void				SaveMe( void );

	void				FireBullet( const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType = TF_DMG_CUSTOM_NONE );
	void				ImpactWaterTrace( trace_t &trace, const Vector &vecStart );
	void				NoteWeaponFired();

	bool				HasItem( void );					// Currently can have only one item at a time.
	void				SetItem( CTFItem *pItem );
	CTFItem				*GetItem( void );

	void				Regenerate( void );
	float				GetNextRegenTime( void ){ return m_flNextRegenerateTime; }
	void				SetNextRegenTime( float flTime ){ m_flNextRegenerateTime = flTime; }

	float				GetNextChangeClassTime(void){ return m_flNextChangeClassTime; }
	void				SetNextChangeClassTime(float flTime){ m_flNextChangeClassTime = flTime; }

	float				GetNextChangeTeamTime(void){ return m_flNextChangeTeamTime; }
	void				SetNextChangeTeamTime(float flTime){ m_flNextChangeTeamTime = flTime; }

	virtual	void		RemoveAllItems( bool removeSuit );
	virtual void		RemoveAllWeapons( void );

	bool				DropCurrentWeapon( void );
	void				DropFlag( void );
	void				TFWeaponRemove( int iWeaponID );
	bool				TFWeaponDrop( CTFWeaponBase *pWeapon, bool bThrowForward );
	virtual bool		BumpWeapon( CBaseCombatWeapon *pWeapon );

	// Class.
	CTFPlayerClass		*GetPlayerClass( void ) 					{ return &m_PlayerClass; }
	int					GetDesiredPlayerClassIndex( void )			{ return m_Shared.m_iDesiredPlayerClass; }
	void				SetDesiredPlayerClassIndex( int iClass )	{ m_Shared.m_iDesiredPlayerClass = iClass; }

	// Team.
	void				ForceChangeTeam( int iTeamNum );
	virtual void		ChangeTeam( int iTeamNum ) { ChangeTeam( iTeamNum, false, false ); }
	virtual void		ChangeTeam( int iTeamNum, bool bAutoTeam, bool bSilent );

	// mp_fadetoblack
	void				HandleFadeToBlack( void );

	// Flashlight controls for SFM - JasonM
	virtual int FlashlightIsOn( void );
	virtual void FlashlightTurnOn( void );
	virtual void FlashlightTurnOff( void );

	// Think.
	virtual void		PreThink();
	virtual void		PostThink();
	virtual bool		HandleInteraction(int interactionType, void *data, CBaseCombatCharacter* sourceEnt);

	virtual void		ItemPostFrame();
	virtual void		Weapon_FrameUpdate( void );
	virtual void		Weapon_HandleAnimEvent( animevent_t *pEvent );
	virtual bool		Weapon_ShouldSetLast( CBaseCombatWeapon *pOldWeapon, CBaseCombatWeapon *pNewWeapon );

	virtual void		GetStepSoundVelocities( float *velwalk, float *velrun );
	virtual void		SetStepSoundTime( stepsoundtimes_t iStepSoundTime, bool bWalking );

	// Utility.
	void				UpdateModel( void );
	void				UpdateSkin( int iTeam );

	virtual int			GiveAmmo( int iCount, int iAmmoIndex, bool bSuppressSound = false );
	virtual int			GiveAmmo( int iCount, int iAmmoIndex, bool bSuppressSound, EAmmoSource ammosource );
	int					GetMaxAmmo( int iAmmoIndex, int iClassNumber = -1 );

	bool				CanAttack( void );

	// This passes the event to the client's and server's CPlayerAnimState.
	void				DoAnimationEvent( PlayerAnimEvent_t event, int mData = 0 );

	virtual bool		ClientCommand( const CCommand &args );
	void				ClientHearVox( const char *pSentence );
	void				DisplayLocalItemStatus( CTFGoal *pGoal );

	bool				CanPickupBuilding( CBaseObject *pObject );
	bool				TryToPickupBuilding( void );

	int					BuildObservableEntityList( void );
	virtual int			GetNextObserverSearchStartPoint( bool bReverse ); // Where we should start looping the player list in a FindNextObserverTarget call
	virtual CBaseEntity *FindNextObserverTarget(bool bReverse);
	virtual bool		IsValidObserverTarget(CBaseEntity * target); // true, if player is allowed to see this target
	virtual bool		SetObserverTarget(CBaseEntity * target);
	virtual bool		ModeWantsSpectatorGUI( int iMode ) { return (iMode != OBS_MODE_FREEZECAM && iMode != OBS_MODE_DEATHCAM); }
	void				FindInitialObserverTarget( void );
	CBaseEntity		    *FindNearestObservableTarget( Vector vecOrigin, float flMaxDist );
	virtual void		ValidateCurrentObserverTarget( void );

	void CheckUncoveringSpies( CTFPlayer *pTouchedPlayer );
	void Touch( CBaseEntity *pOther );

	void TeamFortress_SetSpeed();
	EHANDLE TeamFortress_GetDisguiseTarget( int nTeam, int nClass );

	void TeamFortress_ClientDisconnected();
	void RemoveAllOwnedEntitiesFromWorld( bool bSilent = true );
	void RemoveOwnedProjectiles( void );

	CTFTeamSpawn *GetSpawnPoint( void ){ return m_pSpawnPoint; }
		
	void SetAnimation( PLAYER_ANIM playerAnim );

	bool IsPlayerClass( int iClass ) const;

	void PlayFlinch( const CTakeDamageInfo &info );

	float PlayCritReceivedSound( void );
	void PainSound( const CTakeDamageInfo &info );
	void DeathSound( const CTakeDamageInfo &info );

	// TF doesn't want the explosion ringing sound
	virtual void			OnDamagedByExplosion( const CTakeDamageInfo &info ) { return; }

	void	OnBurnOther( CBaseEntity *pTFPlayerVictim );

	// Buildables
	void SetWeaponBuilder( CTFWeaponBuilder *pBuilder );
	CTFWeaponBuilder *GetWeaponBuilder( void );

	int GetBuildResources( void );
	void RemoveBuildResources( int iAmount );
	void AddBuildResources( int iAmount );

	bool IsBuilding( void );
	int CanBuild( int iObjectType, int iObjectMode );

	CBaseObject	*GetObject( int index );
	int	GetObjectCount( void );
	int GetNumObjects( int iObjectType, int iObjectMode );
	void RemoveAllObjects( bool bSilent );
	void StopPlacement( void );
	int	StartedBuildingObject( int iObjectType );
	void StoppedBuilding( int iObjectType );
	void FinishedObject( CBaseObject *pObject );
	void AddObject( CBaseObject *pObject );
	void OwnedObjectDestroyed( CBaseObject *pObject );
	void RemoveObject( CBaseObject *pObject );
	bool PlayerOwnsObject( CBaseObject *pObject );
	void DetonateOwnedObjectsOfType( int iType, int iMode );
	void StartBuildingObjectOfType( int iType, int iMode );

	CTFTeam *GetTFTeam( void );
	CTFTeam *GetOpposingTFTeam( void );

	void TeleportEffect( void );
	void RemoveTeleportEffect( void );
	void CallForNPCMedic(void);
	bool IsAllowedToPickUpFlag( void );
	bool HasTheFlag( void );
	//bool HasTheFlag( ETFFlagType flagtype, int ?? );

	// Death & Ragdolls.
	virtual void CreateRagdollEntity( void );
	void CreateRagdollEntity( bool bGib, bool bBurning, bool bOnGround, float flInvisLevel, int iDamageCustom, bool bFake );
	void DestroyRagdoll( void );
	//void CreateFeignDeathRagdoll( const CTakeDamageInfo &info, bool ?, bool ?, bool ?)
	//bool FeignDeath( const CTakeDamageInfo &info );
	CNetworkHandle( CBaseEntity, m_hRagdoll );	// networked entity handle 
	virtual bool ShouldGib( const CTakeDamageInfo &info );

	// Dropping stuff
	void DropAmmoPack( void );
	void DropWeapon( CTFWeaponBase *pWeapon, bool bKilled = false );
	void DropRune( void );
	void DropReviveMarker( void );

	bool CanDisguise( void );
	bool CanGoInvisible( void );
	void RemoveInvisibility( void );

	void RemoveDisguise( void );

	bool DoClassSpecialSkill( void );

	float GetLastDamageTime( void ) { return m_flLastDamageTime; }

	void SetClassMenuOpen( bool bIsOpen );
	bool IsClassMenuOpen( void );

	float GetCritMult( void ) { return m_Shared.GetCritMult(); }
	void  RecordDamageEvent( const CTakeDamageInfo &info, bool bKill ) { m_Shared.RecordDamageEvent(info,bKill); }

	bool GetHudClassAutoKill( void ){ return m_bHudClassAutoKill; }
	void SetHudClassAutoKill( bool bAutoKill ){ m_bHudClassAutoKill = bAutoKill; }

	bool GetMedigunAutoHeal( void ){ return m_bMedigunAutoHeal; }
	void SetMedigunAutoHeal( bool bMedigunAutoHeal ){ m_bMedigunAutoHeal = bMedigunAutoHeal; }

	bool ShouldAutoRezoom( void ) { return m_bAutoRezoom; }
	void SetAutoRezoom( bool bAutoRezoom ) { m_bAutoRezoom = bAutoRezoom; }

	bool ShouldAutoReload( void ) { return m_bAutoReload; }
	void SetAutoReload( bool bAutoReload ) { m_bAutoReload = bAutoReload; }

	bool ShouldFlipViewModel( void ) { return m_bFlipViewModel; }
	void SetFlipViewModel( bool bFlip ) { m_bFlipViewModel = bFlip; }

	virtual void	ModifyOrAppendCriteria( AI_CriteriaSet& criteriaSet );

	virtual bool CanHearAndReadChatFrom( CBasePlayer *pPlayer );

	Vector 	GetClassEyeHeight( void );

	void	UpdateExpression( void );
	void	ClearExpression( void );

	void	AddPhaseEffects( void );

	virtual IResponseSystem *GetResponseSystem();
	virtual bool			SpeakConceptIfAllowed( int iConcept, const char *modifiers = NULL, char *pszOutResponseChosen = NULL, size_t bufsize = 0, IRecipientFilter *filter = NULL );

	virtual bool CanSpeakVoiceCommand( void );
	virtual bool ShouldShowVoiceSubtitleToEnemy( void );
	virtual void NoteSpokeVoiceCommand( const char *pszScenePlayed );
	void	SpeakWeaponFire( int iCustomConcept = MP_CONCEPT_NONE );
	void	ClearWeaponFireScene( void );

	virtual int DrawDebugTextOverlays( void );

	float m_flNextVoiceCommandTime;
	float m_flNextSpeakWeaponFire;

	virtual int	CalculateTeamBalanceScore( void );

	bool ShouldAnnouceAchievement( void );

	virtual void		PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force );
	virtual bool		IsDeflectable( void ) { return true; }

	virtual CAttributeManager *GetAttributeManager( void ) { return &m_AttributeManager; }
	virtual CAttributeContainer *GetAttributeContainer( void ) { return NULL; }
	virtual CBaseEntity *GetAttributeOwner( void ) { return NULL; }
	virtual void ReapplyProvision( void ) { /*Do nothing*/ };

	void UpdatePlayerColor( void );

	// Entity inputs
	void	InputIgnitePlayer( inputdata_t &inputdata );
	void	InputExtinguishPlayer( inputdata_t &inputdata );
	void	InputBleedPlayer( inputdata_t &inputdata );
	void	InputSpeakResponseConcept( inputdata_t &inputdata );
	void	InputSetForcedTauntCam( inputdata_t &inputdata );

	// HL2 ladder related methods
	LadderMove_t		*GetLadderMove() { return &m_LadderMove; }
	virtual void		ExitLadder();
	virtual surfacedata_t *GetLadderSurface( const Vector &origin );

	// physics interactions
	virtual void		PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize );
	virtual	bool		IsHoldingEntity( CBaseEntity *pEnt );
	virtual void		ForceDropOfCarriedPhysObjects( CBaseEntity *pOnlyIfHoldindThis );
	virtual float		GetHeldObjectMass( IPhysicsObject *pHeldObject );

	virtual bool		IsFollowingPhysics( void ) { return (m_afPhysicsFlags & PFLAG_ONBARNACLE) > 0; }
	void				InputForceDropPhysObjects( inputdata_t &data );

	// Required for func_tank and some other things.
	virtual Vector		EyeDirection2D( void );
	virtual Vector		EyeDirection3D( void );

	// Commander Mode for controller NPCs
	enum CommanderCommand_t
	{
		CC_NONE,
		CC_TOGGLE,
		CC_FOLLOW,
		CC_SEND,
	};

	virtual CAI_Squad	*GetPlayerSquad() { return m_pPlayerAISquad; }
	virtual CAI_Squad	*GetPlayerSquad() const { return m_pPlayerAISquad; }
	virtual void CommanderMode();
	void CommanderUpdate();
	void CommanderExecute( CommanderCommand_t command = CC_TOGGLE );
	bool CommanderFindGoal( commandgoal_t *pGoal );
	void NotifyFriendsOfDamage( CBaseEntity *pAttackerEntity );
	CAI_BaseNPC *GetSquadCommandRepresentative();
	int GetNumSquadCommandables();
	int GetNumSquadCommandableMedics();

	//Transition
	void	SaveForTransition( void );
	void	DeleteForTransition( void );

	virtual void ResetPerRoundStats( void );
	virtual void UpdatePlayerSound( void );

	bool IsOnStoryTeam( void ) { return ( GetTeamNumber() == TF_STORY_TEAM ); }
	bool IsOnCombineTeam(void) { return (GetTeamNumber() == TF_COMBINE_TEAM); }

	char GetPrevTextureType( void ) { return m_chPreviousTextureType; }

	Class_T				Classify ( void );

	void FirePlayerProxyOutput( const char *pszOutputName, variant_t variant, CBaseEntity *pActivator, CBaseEntity *pCaller );

	CLogicPlayerProxy	*GetPlayerProxy( void );

	void MissedAR2AltFire();

	inline void EnableCappedPhysicsDamage();
	inline void DisableCappedPhysicsDamage();

	CSoundPatch *m_sndLeeches;
	CSoundPatch *m_sndWaterSplashes;

	const impactdamagetable_t &GetPhysicsImpactDamageTable();

	void			ApplyAbsVelocityImpulse( const Vector &vecImpulse );
	virtual void	ApplyGenericPushbackImpulse( const Vector &vecDir );

	void		FeignDeath( const CTakeDamageInfo &info );
public:

	CNetworkVector( m_vecPlayerColor );

	CTFPlayerShared m_Shared;

	int	    item_list;			// Used to keep track of which goalitems are 
								// affecting the player at any time.
								// GoalItems use it to keep track of their own 
								// mask to apply to a player's item_list

	float invincible_finished;
	float invisible_finished;
	float super_damage_finished;
	float radsuit_finished;

	int m_flNextTimeCheck;		// Next time the player can execute a "timeleft" command

	// TEAMFORTRESS VARIABLES
	int		no_sentry_message;
	int		no_dispenser_message;
	
	CNetworkVar( bool, m_bSaveMeParity );
	CNetworkVar(bool, m_bHasLongJump);

	// teleporter variables
	int		no_entry_teleporter_message;
	int		no_exit_teleporter_message;

	float	m_flNextNameChangeTime;

	bool	m_bBlastLaunched;
	bool	m_bAirblasted;

	bool	m_bIsPlayerADev;
	bool	m_bIsPlayerNicknine;

	int					StateGet( void ) const;

	void				SetOffHandWeapon( CTFWeaponBase *pWeapon );
	void				HolsterOffHandWeapon( void );

	float				GetSpawnTime() { return m_flSpawnTime; }

	virtual bool Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex = 0 );
	virtual void Weapon_Drop( CBaseCombatWeapon *pWeapon, const Vector *pvecTarget , const Vector *pVelocity );

	bool				ItemsMatch( CEconItemView *pItem1, CEconItemView *pItem2, CTFWeaponBase *pWeapon );
	void				ValidateWeapons( bool bRegenerate );
	void				ValidateWearables( void );
	void				ManageRegularWeapons( TFPlayerClassData_t *pData );
	void				ManageRegularWeaponsLegacy( TFPlayerClassData_t *pData );
	void				ManageRandomWeapons( TFPlayerClassData_t *pData );
	void				ManageBuilderWeapons( TFPlayerClassData_t *pData );
	void				ManageTeamWeapons( TFPlayerClassData_t *pData );
	void				ManageGrenades(TFPlayerClassData_t *pData);

	void				PostInventoryApplication( void );

	// Taunts.
	void				Taunt( void );
	bool				IsTaunting( void ) { return m_Shared.InCond( TF_COND_TAUNTING ); }
	void				DoTauntAttack( void );
	void				ClearTauntAttack( void );
	QAngle				m_angTauntCamera;

	virtual float		PlayScene( const char *pszScene, float flDelay = 0.0f, AI_Response *response = NULL, IRecipientFilter *filter = NULL );
	void				ResetTauntHandle( void )				{ m_hTauntScene = NULL; }
	void				SetDeathFlags( int iDeathFlags ) { m_iDeathFlags = iDeathFlags; }
	int					GetDeathFlags() { return m_iDeathFlags; }
	void				SetMaxSentryKills( int iMaxSentryKills ) { m_iMaxSentryKills = iMaxSentryKills; }
	int					GetMaxSentryKills() { return m_iMaxSentryKills; }

	CNetworkVar( int, m_iSpawnCounter );
	
	void				CheckForIdle( void );
	void				PickWelcomeObserverPoint();

	void				StopRandomExpressions( void ) { m_flNextRandomExpressionTime = -1; }
	void				StartRandomExpressions( void ) { m_flNextRandomExpressionTime = gpGlobals->curtime; }

	virtual bool			WantsLagCompensationOnEntity( const CBaseEntity	*pEntity, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits ) const;

	float				MedicGetChargeLevel( void );

	CWeaponMedigun		*GetMedigun( void );
	CTFWeaponBase		*Weapon_OwnsThisID( int iWeaponID );
	CTFWeaponBase		*Weapon_GetWeaponByType( int iType );
	CEconEntity			*GetEntityForLoadoutSlot( int iSlot );
	CEconWearable		*GetWearableForLoadoutSlot( int iSlot );

	bool CalculateAmmoPackPositionAndAngles( CTFWeaponBase *pWeapon, Vector &vecOrigin, QAngle &vecAngles );

	bool				SelectFurthestSpawnSpot( const char *pEntClassName, CBaseEntity* &pSpot, bool bTelefrag = true );

	// Vintage Gunslinger
	bool				HasGunslinger( void ) { return m_Shared.m_bGunslinger; }

	virtual int			GetMaxHealth( void ) const;

	float				GetDesiredHeadScale( void );
	float				GetDesiredTorsoScale( void );
	float				GetDesiredHandScale( void );
	float				GetHeadScaleSpeed( void );
	float				GetTorsoScaleSpeed( void );
	float				GetHandScaleSpeed( void );
	float				m_flHeadScale;
	float				m_flTorsoScale;
	float				m_flHandScale;
private:

	int					GetAutoTeam( void );

	// Creation/Destruction.
	void				InitClass( void );
	void				GiveDefaultItems();
	bool				SelectSpawnSpot( const char *pEntClassName, CBaseEntity* &pSpot );
	void				SearchCoopSpawnSpot( void );
	bool				ShouldUseCoopSpawning( void );
	void				PrecachePlayerModels( void );
	void				RemoveNemesisRelationships();

	// Think.
	void				TFPlayerThink();
	void				RegenThink();
	void				UpdateTimers( void );

	// Taunt.
	EHANDLE				m_hTauntScene;
	bool				m_bInitTaunt;

	// Client commands.
	void				HandleCommand_JoinTeam( const char *pTeamName );
	void				HandleCommand_JoinClass( const char *pClassName );
	void				HandleCommand_JoinTeam_NoMenus( const char *pTeamName );
	void				HandleCommand_JoinTeam_NoKill( const char *pTeamName );

	// Bots.
	friend void			Bot_Think( CTFPlayer *pBot );

	// Physics.
	void				PhysObjectSleep();
	void				PhysObjectWake();

	// Ammo pack.
	void AmmoPackCleanUp( void );
	void DroppedWeaponCleanUp( void );

	// State.
	CPlayerStateInfo	*StateLookupInfo( int nState );
	void				StateEnter( int nState );
	void				StateLeave( void );
	void				StateTransition( int nState );
	void				StateEnterWELCOME( void );
	void				StateThinkWELCOME( void );
	void				StateEnterPICKINGTEAM( void );
	void				StateEnterACTIVE( void );
	void				StateEnterOBSERVER( void );
	void				StateThinkOBSERVER( void );
	void				StateEnterDYING( void );
	void				StateThinkDYING( void );

	virtual bool		SetObserverMode(int mode);
	virtual void		AttemptToExitFreezeCam( void );

	bool				PlayGesture( const char *pGestureName );
	bool				PlaySpecificSequence( const char *pSequenceName );
	bool				PlayDeathAnimation( const CTakeDamageInfo &info, CTakeDamageInfo &info_modified );

	bool				GetResponseSceneFromConcept( int iConcept, char *chSceneBuffer, int numSceneBufferBytes );

	bool				CommanderExecuteOne( CAI_BaseNPC *pNpc, const commandgoal_t &goal, CAI_BaseNPC **Allies, int numAllies );

	void				AddCustomAttribute( char const *pAttribute, float flValue /*, float *pValue?*/ );
	//void				UpdateCustomAttributes( void );
	void				RemoveCustomAttribute( char const *pAttribute );

	//void				RemovePlayerAttributes( bool bRemove? );
private:
	// Map introductions
	int					m_iIntroStep;
	CHandle<CIntroViewpoint> m_hIntroView;
	float				m_flIntroShowHintAt;
	float				m_flIntroShowEventAt;
	bool				m_bHintShown;
	bool				m_bAbortFreezeCam;
	bool				m_bSeenRoundInfo;
	bool				m_bRegenerating;

	// Items.
	CNetworkHandle( CTFItem, m_hItem );

	// Combat.
	CNetworkHandle( CTFWeaponBase, m_hOffHandWeapon );

	float					m_flHealthBuffTime;

	float					m_flNextRegenerateTime;
	float					m_flNextChangeClassTime;
	float					m_flNextChangeTeamTime;
	float					m_flNextHealthRegen;

	// Ragdolls.
	Vector					m_vecTotalBulletForce;

	// State.
	CPlayerStateInfo		*m_pStateInfo;

	// Spawn Point
	CTFTeamSpawn			*m_pSpawnPoint;

	// Networked.
	CNetworkQAngle( m_angEyeAngles );					// Copied from EyeAngles() so we can send it to the client.

	CTFPlayerClass		m_PlayerClass;
	int					m_WeaponPreset[TF_CLASS_COUNT_ALL][TF_LOADOUT_SLOT_COUNT];

	CTFPlayerAnimState	*m_PlayerAnimState;
	int					m_iLastWeaponFireUsercmd;				// Firing a weapon.  Last usercmd we shot a bullet on.
	int					m_iLastSkin;

	CNetworkVar( float, m_flLastDamageTime );

	float				m_flNextPainSoundTime;
	int					m_LastDamageType;
	int					m_iDeathFlags;				// TF_DEATH_* flags with additional death info
	int					m_iMaxSentryKills;			// most kills by a single sentry

	bool				m_bPlayedFreezeCamSound;

	CHandle< CTFWeaponBuilder > m_hWeaponBuilder;

	CUtlVector<EHANDLE>	m_aObjects;			// List of player objects

	bool m_bIsClassMenuOpen;

	Vector m_vecLastDeathPosition;

	float				m_flSpawnTime;

	float				m_flLastAction;
	bool				m_bIsIdle;

	CUtlVector<EHANDLE>	m_hObservableEntities;
	DamagerHistory_t m_DamagerHistory[MAX_DAMAGER_HISTORY];	// history of who has damaged this player
	CUtlVector<float>	m_aBurnOtherTimes;					// vector of times this player has burned others

	// Background expressions
	string_t			m_iszExpressionScene;
	EHANDLE				m_hExpressionSceneEnt;
	float				m_flNextRandomExpressionTime;
	EHANDLE				m_hWeaponFireSceneEnt;

	bool				m_bSpeakingConceptAsDisguisedSpy;

	bool				m_bHudClassAutoKill;
	bool 				m_bMedigunAutoHeal;
	bool				m_bAutoRezoom;	// does the player want to re-zoom after each shot for sniper rifles
	bool				m_bAutoReload;
	bool				m_bFlipViewModel;

	float				m_flTauntAttackTime;
	int					m_iTauntAttack;

	// Gunslinger taunt
	short				m_nTauntDamageCount;

	float				m_flNextCarryTalkTime;

	EHANDLE				m_hTempSpawnSpot;
	CHandle<CTFReviveMarker> m_hReviveSpawnSpot;
	CNetworkVar( bool, m_bSearchingSpawn )

	int					m_nBlastJumpFlags;
	bool				m_bJumpEffect;

	CNetworkVar( int, m_nForceTauntCam );
	CNetworkVar( bool, m_bTyping );

	CNetworkVarEmbedded( CAttributeManager, m_AttributeManager );

	COutputEvent		m_OnDeath;

	// HL2 squad stuff
	CAI_Squad *			m_pPlayerAISquad;
	CSimpleSimTimer		m_CommanderUpdateTimer;
	float				m_RealTimeLastSquadCommand;
	CommanderCommand_t	m_QueuedCommand;

	CNetworkVar( int,	m_iSquadMemberCount );
	CNetworkVar( int,	m_iSquadMedicCount );
	CNetworkVar( bool,	m_fSquadInFollowMode );

	// Suit power fields
	float				m_flSuitPowerLoad;	// net suit power drain (total of all device's drainrates)
	float				m_flAdmireGlovesAnimTime;

	float				m_flNextFlashlightCheckTime;
	float				m_flFlashlightPowerDrainScale;

	EHANDLE				m_hPlayerProxy;

	bool				m_bFlashlightDisabled;
	bool				m_bUseCappedPhysicsDamageTable;
public:
	bool				SetPowerplayEnabled( bool bOn );
	bool				PlayerHasPowerplay( void );
	bool				PlayerIsDevTrain( void );
	bool				IsDeveloper( void ) { return m_bIsPlayerADev; }
	bool				IsNicknine( void );
	void				PowerplayThink( void );
	float				m_flPowerPlayTime;
	
	// HL2 Ladder related data
	CNetworkVar( EHANDLE, m_hLadder );
	LadderMove_t			m_LadderMove;
	bool				m_bTransition;
};

//-----------------------------------------------------------------------------
// FIXME: find a better way to do this
// Switches us to a physics damage table that caps the max damage.
//-----------------------------------------------------------------------------
void CTFPlayer::EnableCappedPhysicsDamage()
{
	m_bUseCappedPhysicsDamageTable = true;
}


void CTFPlayer::DisableCappedPhysicsDamage()
{
	m_bUseCappedPhysicsDamageTable = false;
}

//-----------------------------------------------------------------------------
// Purpose: Utility function to convert an entity into a tf player.
//   Input: pEntity - the entity to convert into a player
//-----------------------------------------------------------------------------
inline CTFPlayer *ToTFPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	Assert( dynamic_cast<CTFPlayer*>( pEntity ) != 0 );
	return static_cast< CTFPlayer* >( pEntity );
}

inline int CTFPlayer::StateGet( void ) const
{
	return m_Shared.m_nPlayerState;
}



#endif	// TF_PLAYER_H
