/*coded by https://github.com/soamane*/

#define RELEASE_CLASS(x, y) x = y; if (!x) return;

#include "stdafx.h"

#pragma comment(linker,"/NODEFAULTLIB")


bool CheckTeam( Actor* pActor, Actor* p_mActor ) {
    return p_mActor != pActor && p_mActor->GetActorTeam( ) != pActor->GetActorTeam( ) || p_mActor->GetActorTeam( ) == 0;
}

void DrawMarker( Entity* pEntity, GameRules* pRules, SMarkerParams pParams, const char* name ) {
    FeedBackSystem* feedback = pRules->GetFeedbackSystem( );
    if ( !feedback ) return;
    feedback->CreateMarker( name, &pParams );
}


Hook hook;
using uhook = bool( __stdcall* )( SystemGlobalEnviroment::System* system, Update update, int reserve );
uhook ReleaseHook{};
void* pointer;

void Release( ) {
    SystemGlobalEnviroment* pEnviroment;          RELEASE_CLASS( pEnviroment, SystemGlobalEnviroment::GlobalEnviroment( ) );
    Console* pConsole;                            RELEASE_CLASS( pConsole, pEnviroment->GetConsole( ) );
    Engine* pEngine;                              RELEASE_CLASS( pEngine, pEnviroment->Get3DEgnine( ) );
    EntitySystem* pEntitySystem;                  RELEASE_CLASS( pEntitySystem, pEnviroment->GetEntitySystem( ) );
    EntityIterator* pEntityIt;                    RELEASE_CLASS( pEntityIt, pEntitySystem->GetEntityIterator( ) );
    Game* pGame;                                  RELEASE_CLASS( pGame, pEnviroment->GetGame( ) );
    GameFramework* pFrame;                        RELEASE_CLASS( pFrame, pGame->GetGameFramework( ) );
    GameRules* pGameRules;                        RELEASE_CLASS( pGameRules, pFrame->GetGameRules( ) );
    Actor* pClientActor;                          RELEASE_CLASS( pClientActor, pFrame->GetClientActor( ) );
    Input* pInput;                                RELEASE_CLASS( pInput, pEnviroment->GetInput( ) );

   /* pClientActor->SetActorModelFiles( "objects/characters/presets/sentry_gun/sentry_gun.cpf" );
    pClientActor->SetActorModel( );*/
    pConsole->ResetDelay( );
    pEngine->SetEffect( "FlashBang_Active", false );

    if ( pInput->GetKeyState( "F4", KeyEvent::eDown ) )
    {
        SMarkerParams m_param;
        setmemory( &m_param, 0, sizeof( SMarkerParams ) );

        for ( ; Entity * entity = pEntityIt->GetEntityIterator( );)
        {
            if ( m_param.m_entity = entity->GetEntity( ) )
            {
                if ( Actor* it_actor = pFrame->GetActorSystem( )->GetActor( m_param.m_entity ) )
                {
                    if ( CheckTeam( pClientActor, it_actor ) )
                    {
                        DrawMarker( entity, pGameRules, m_param, "hunt_enemy_global_spotting" );
                    }
                }
            }
        }
    }
}
bool __stdcall Code( SystemGlobalEnviroment::System* system, Update update, int reserve ) {
    Release( );
    return ReleaseHook( system, update, reserve );
}

bool __stdcall _DllMainCRTStartup( HINSTANCE hmodule, DWORD reason, LPVOID reserve ) {

    if ( reason == 1 )
    {
        pointer = SystemGlobalEnviroment::GlobalEnviroment( )->GetSystem( );
        ReleaseHook = *(decltype( ReleaseHook )*)( *(int64_t*)pointer + 4 * sizeof( void* ) );
        hook.SetInstall( pointer );
        hook.SetHook( 4, Code );
    }

    return true;
}