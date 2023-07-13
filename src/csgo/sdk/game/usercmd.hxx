#pragma once

#include "../../../includes.hxx"

class CUserCmd
{
public:

	virtual ~CUserCmd() { };

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	Vector	viewangles;
	Vector	aimdirection;	// For pointing devices. 
	// Intended velocities
	//	forward velocity.
	float	forwardmove;
	//  sideways velocity.
	float	sidemove;
	//  upward velocity.
	float	upmove;
	// Attack button states
	int		buttons;
	// Impulse command issued.
	byte    impulse;
	// Current weapon id
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	// For shared random functions

#ifndef CLIENT_DLL
	int		server_random_seed; // Only the server populates this seed
#endif

	short	mousedx;		// mouse accum in x from create move
	short	mousedy;		// mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool	hasbeenpredicted;

	// Back channel to communicate IK state
#if defined( HL2_DLL ) || defined( HL2_CLIENT_DLL )
	CUtlVector< CEntityGroundContact > entitygroundcontact;
#endif

#if defined ( DOTA_DLL )
	CUnitOrders dota_unitorders;
#endif

#if defined ( PORTAL2 )
	// Portal 2's grab code is on the client to support multiplayer
	short player_held_entity;
	// This one is temporary-- some server code needs to know if this trace 
	// went through a portal. This should go away when we move the grabcontrollers
	// down to the client as well.
	short held_entity_was_grabbed_through_portal;

	unsigned short command_acknowledgements_pending; //so we can properly sync portal teleportation angle changes. The server tells us the last command it acknowledged, now we also tell it how many acknowledgments we're waiting on (command_number - engine->GetLastAcknowledgedCommand())
	uint8 predictedPortalTeleportations; //should probably enumerate which transforms we went through if we want perfect accuracy
#endif // PORTAL2

	// TrackIR
	Vector headangles;
	Vector headoffset;
	// TrackIR

#if defined( INFESTED_DLL ) || defined( DOTA_DLL )
	Vector crosshairtrace;		// world location directly beneath the player's crosshair
#endif

#ifdef INFESTED_DLL
	short crosshair_entity;			// index of the entity under the player's crosshair
	byte forced_action;
	short sync_kill_ent;
	Vector skill_dest;
	short skill_dest_ent;
#endif

#if defined( KEEP_COMMAND_REPREDICTION_COUNT )
	unsigned int debug_RepredictionCount;
#endif
};