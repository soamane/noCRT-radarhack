/*coded by https://github.com/soamane*/

#pragma once
extern "C" float _fltused = 0;

#include <windows.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <cmath>
#include <fstream>

template <typename template_table> template_table _virtuàl( void* v_called, __int64 v_index ) { return ( *reinterpret_cast<template_table**>( v_called ) )[v_index]; }

template  <int64_t v_index, typename v_type, typename... v_args>
v_type v_table( void* v_void, v_args... v_arguments ) {
	using UsngVirtual = v_type( __fastcall* )( void*, v_args... );
	auto ReturnFunction = ( *reinterpret_cast<UsngVirtual**>( v_void ) )[v_index];
	return  ReturnFunction( v_void, v_arguments... );
}

//template <typename str, typename str_args>
//void check_pointers( str pointer, str_args arg... ) {
//	if ( !pointer & !arg ) return;
//}
#include "math.h"
#include "classes.h"
