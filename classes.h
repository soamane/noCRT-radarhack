/*coded by https://github.com/soamane*/

#pragma once
#include "stdafx.h"
typedef unsigned int EntityId;

int64_t GlobalEnvironment = 0x141F31680;
int64_t AddMarker = 0x1410C5D40;

struct FeedbackParams {
    virtual void args0( ) {};
    virtual void args1( ) {};
    virtual void args2( ) {};
    virtual void args3( ) {};
    virtual void args4( ) {};
};

struct Vec3 {
    float x, y, z;
};
struct SMarkerParams : FeedbackParams {
    unsigned int m_entity;
    Vec3 m_pos;
};

enum KeyEvent { eDown = 4 };

struct CKey {
    const char* c_key;
    CKey( ) { c_key = ""; }
    CKey( const char* nkey ) { c_key = nkey; };
    operator const char* ( ) const { return c_key; };
};

class Entity {
public:
    UINT GetEntity( ) {
        return v_table<0x1, UINT>( this );
    }
};

class EntityIterator {
public:

    Entity* GetEntityIterator( ) {
        return v_table<0x6, Entity*>( this );
    }
};

class EntitySystem {
public:
    EntityIterator* GetEntityIterator( ) {
        return v_table<0x15, EntityIterator*>( this );
    }
};

class FeedBackSystem {
public:
    using marker = signed __int64( __thiscall* )( FeedBackSystem*, const char*, const SMarkerParams* );
    signed __int64 CreateMarker( const char* name, const SMarkerParams* params ) {
        return marker( AddMarker )( this, name, params );
    }
};

class Actor {
public:

    EntityId GetEntityId( ) {
        return *(EntityId*)( (int)this + ( 0x1 ) );
    }
    int64_t GetActorClass( ) {
        return *reinterpret_cast<int64_t*>( this + ( 0x458 ) );
    }
    int64_t GetActorTeam( ) {
        return *reinterpret_cast<int64_t*>( this + ( 0x3E0 ) );
    }
    int64_t GetEntity( ) {
        return *reinterpret_cast<int64_t*>( this + ( 0x20 ) );
    }

};
class ActorSystem {
public:
    Actor* GetActor( UINT EntityId ) {
        return v_table<0xF, Actor*>( this, EntityId );
    }
};

class GameRules {
public:
    FeedBackSystem* GetFeedbackSystem( ) {
        return *reinterpret_cast<FeedBackSystem**>( this + ( 0x300 ) );
    }
};

class GameFramework {
public:
    Actor* GetClientActor( ) {
        Actor* pLocalActor = 0;

        bool bGetActorResult = v_table<0x470 / 8, bool>( this, &pLocalActor );
        if ( !bGetActorResult ) pLocalActor = 0;
        return pLocalActor;
    }
    ActorSystem* GetActorSystem( ) {
        return v_table<0x19, ActorSystem*>( this );
    }
    GameRules* GetGameRules( ) {
        return v_table<0x89, GameRules*>( this );
    }
};

class Game {
public:
    GameFramework* GetGameFramework( ) {
        return v_table< 0xE, GameFramework*>( this );
    }
};

class Console {
public:
    void ResetDelay( ) {
        GetString( ( "cl_coop_mode_switch_delay 0" ) );
        GetString( ( "cl_crouch_spam_counter 0" ) );
        GetString( ( "cl_crouch_spam_gap_time 0" ) );
        GetString( ( "cl_crouch_spam_punish_timeout 0" ) );
        GetString( ( "cl_prone_hopping_improvement 0" ) );
        GetString( ( "cl_coop_assist_spam_gap_time 0" ) );
        GetString( ( "cl_coop_assist_spam_punish_timeout 0" ) );
        GetString( ( "cl_prone_hopping_timeout 0" ) );
        GetString( ( "cl_prone_spam_punish_timeout 0" ) );
    }
private:
    void GetString( const char* name, bool state = true, bool execution = false ) {
        return v_table<35, void>( this, name, state, execution );
    }
};

class Engine {
public:
    void SetEffect( const char* name, float value, bool force = false ) {
        v_table<145, void>( this, name, value, force );
    }
};

class Input {
public:
    bool GetKeyState( const CKey& c_key, int c_state ) { return _virtuàl<bool( __thiscall* )( void*, const CKey&, int )>( this, 0x15 )( this, c_key, c_state ); }
};

class SystemGlobalEnviroment {
public: class System {};
      System* GetSystem( ) {
          return *reinterpret_cast<System**>( this + ( 0xF0 ) );
      }
      Engine* Get3DEgnine( ) {
          return *reinterpret_cast<Engine**>( this + ( 0x100 ) );
      }
      Console* GetConsole( ) {
          return *reinterpret_cast<Console**>( this + ( 0xC8 ) );
      }
      Game* GetGame( ) {
          return *reinterpret_cast<Game**>( this + ( 0xD0 ) );
      }
      EntitySystem* GetEntitySystem( ) {
          return *reinterpret_cast<EntitySystem**>( this + ( 0xE0 ) );
      }
      Input* GetInput( ) {
          return *reinterpret_cast<Input**>( this + ( 0x78 ) );
      }

      static SystemGlobalEnviroment* GlobalEnviroment( ) {
          return *reinterpret_cast<SystemGlobalEnviroment**>( GlobalEnvironment );
      }
};
enum Update {};

void* __cdecl setmemory( void* pointer, int m_value, size_t size_num ) {
    __stosb( static_cast<unsigned char*>( pointer ),
        static_cast<unsigned char>( m_value ), size_num );
    return pointer;
}
void* __cdecl cpymemory( void* pointer, const void* m_void, size_t size_num ) {
    __movsb( static_cast<unsigned char*>( pointer ),
        static_cast<const unsigned char*>( m_void ), size_num );
    return pointer;
}

struct Hook {
    void* c_code;
    void* c_table;
    void* c_allocate[1024];

    void SetInstall( void* pointer, int size = 1024 ) {
        this->c_code = static_cast<int64_t**>( pointer );
        if ( size == NULL ) return;
        cpymemory( c_allocate, *reinterpret_cast<void**>( c_code ), size * sizeof uintptr_t );
        c_table = *reinterpret_cast<void**>( c_code );
        *reinterpret_cast<void**>( c_code ) = c_allocate;
    }
    void* SetHook( int h_index, void* h_function ) {
        *reinterpret_cast<void**>( (__int64*)c_allocate + h_index ) = h_function;
        return *reinterpret_cast<void**>( (__int64*)c_table + h_index );

    }
};

