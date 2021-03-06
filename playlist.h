/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *  Copyright (C) 2016-2019 - Brad Parker
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PLAYLIST_H__
#define _PLAYLIST_H__

#include <stddef.h>

#include <retro_common_api.h>
#include <boolean.h>
#include <lists/string_list.h>

RETRO_BEGIN_DECLS

typedef struct content_playlist playlist_t;

enum playlist_runtime_status
{
   PLAYLIST_RUNTIME_UNKNOWN = 0,
   PLAYLIST_RUNTIME_MISSING,
   PLAYLIST_RUNTIME_VALID
};

enum playlist_file_mode
{
    PLAYLIST_LOAD,
    PLAYLIST_SAVE
};

struct playlist_entry
{
   char *path;
   char *label;
   char *core_path;
   char *core_name;
   char *db_name;
   char *crc32;
   char *subsystem_ident;
   char *subsystem_name;
   struct string_list *subsystem_roms;
   enum playlist_runtime_status runtime_status;
   unsigned runtime_hours;
   unsigned runtime_minutes;
   unsigned runtime_seconds;
   /* Note: due to platform dependence, have to record
    * timestamp as either a string or independent integer
    * values. The latter is more verbose, but more efficient. */
   unsigned last_played_year;
   unsigned last_played_month;
   unsigned last_played_day;
   unsigned last_played_hour;
   unsigned last_played_minute;
   unsigned last_played_second;
};

/**
 * playlist_init:
 * @path            	   : Path to playlist contents file.
 * @size                : Maximum capacity of playlist size.
 *
 * Creates and initializes a playlist.
 *
 * Returns: handle to new playlist if successful, otherwise NULL
 **/
playlist_t *playlist_init(const char *path, size_t size);

/**
 * playlist_free:
 * @playlist        	   : Playlist handle.
 *
 * Frees playlist handle.
 */
void playlist_free(playlist_t *playlist);

/**
 * playlist_clear:
 * @playlist        	   : Playlist handle.
 *
 * Clears all playlist entries in playlist.
 **/
void playlist_clear(playlist_t *playlist);

/**
 * playlist_size:
 * @playlist        	   : Playlist handle.
 *
 * Gets size of playlist.
 * Returns: size of playlist.
 **/
size_t playlist_size(playlist_t *playlist);

/**
 * playlist_get_index:
 * @playlist               : Playlist handle.
 * @idx                 : Index of playlist entry.
 *
 * Gets values of playlist index:
 **/
void playlist_get_index(playlist_t *playlist,
      size_t idx,
      const struct playlist_entry **entry);

/**
 * playlist_delete_index:
 * @playlist               : Playlist handle.
 * @idx                 : Index of playlist entry.
 *
 * Deletes the entry at index:
 **/
void playlist_delete_index(playlist_t *playlist,
      size_t idx);

/**
 * playlist_resolve_path:
 * @mode      : PLAYLIST_LOAD or PLAYLIST_SAVE
 * @path        : The path to be modified
 *
 * Resolves the path of an item, such as the content path or path to the core, to a format
 * appropriate for saving or loading depending on the @mode parameter
 *
 * Can be platform specific. File paths for saving can be abbreviated to avoid saving absolute
 * paths, as the base directory (home or application dir) may change after each subsequent
 * install (iOS)
 **/
void playlist_resolve_path(enum playlist_file_mode mode,
      char *path, size_t size);

/**
 * playlist_push:
 * @playlist        	   : Playlist handle.
 * @path                : Path of new playlist entry.
 * @core_path           : Core path of new playlist entry.
 * @core_name           : Core name of new playlist entry.
 *
 * Push entry to top of playlist.
 **/
bool playlist_push(playlist_t *playlist,
      const struct playlist_entry *entry);

bool playlist_push_runtime(playlist_t *playlist,
      const struct playlist_entry *entry);

void playlist_update(playlist_t *playlist, size_t idx,
      const struct playlist_entry *update_entry);

/* Note: register_update determines whether the internal
 * 'playlist->modified' flag is set when updating runtime
 * values. Since these are normally set temporarily (for
 * display purposes), we do not always want this function
 * to trigger a re-write of the playlist file. */
void playlist_update_runtime(playlist_t *playlist, size_t idx,
      const struct playlist_entry *update_entry,
      bool register_update);

void playlist_get_index_by_path(playlist_t *playlist,
      const char *search_path,
      const struct playlist_entry **entry);

bool playlist_entry_exists(playlist_t *playlist,
      const char *path,
      const char *crc32);

char *playlist_get_conf_path(playlist_t *playlist);

uint32_t playlist_get_size(playlist_t *playlist);

void playlist_write_file(playlist_t *playlist);

void playlist_write_runtime_file(playlist_t *playlist);

void playlist_qsort(playlist_t *playlist);

void playlist_free_cached(void);

playlist_t *playlist_get_cached(void);

bool playlist_init_cached(const char *path, size_t size);

void command_playlist_push_write(
      playlist_t *playlist,
      const struct playlist_entry *entry);

void command_playlist_update_write(
      playlist_t *playlist,
      size_t idx,
      const struct playlist_entry *entry);

/* Returns true if specified playlist index matches
 * specified content/core paths */
bool playlist_index_is_valid(playlist_t *playlist, size_t idx,
      const char *path, const char *core_path);

void playlist_get_crc32(playlist_t *playlist, size_t idx,
      const char **crc32);

/* If db_name is empty, 'returns' playlist file basename */
void playlist_get_db_name(playlist_t *playlist, size_t idx,
      const char **db_name);

char *playlist_get_default_core_path(playlist_t *playlist);
char *playlist_get_default_core_name(playlist_t *playlist);

void playlist_set_default_core_path(playlist_t *playlist, const char *core_path);
void playlist_set_default_core_name(playlist_t *playlist, const char *core_name);

RETRO_END_DECLS

#endif
