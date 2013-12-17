/* vim:set cin ft=c sw=4 sts=4 ts=8 et ai cino=Ls\:0t0(0 : -*- mode:c;fill-column:80;tab-width:8;c-basic-offset:4;indent-tabs-mode:nil;c-file-style:"k&r" -*-*/
/* Last modified by Sean Hunt, 2013-12-10 */
/* Copyright (c) Daniel Thaler, 2012 */
/* NetHack may be freely redistributed.  See license for details. */

#include "nhcurses.h"

void
net_loadgame(void)
{
    char buf[BUFSZ];
    struct nhnet_game *gamelist;
    struct nh_menuitem *items;
    int size, icount, id, i, n, ret, pick[1];

    gamelist = nhnet_list_games(FALSE, FALSE, &size);
    if (!size) {
        curses_msgwin("No saved games found.");
        return;
    }

    icount = 0;
    items = malloc(size * sizeof (struct nh_menuitem));
    for (i = 0; i < size; i++) {
        if (gamelist[i].status == LS_DONE || gamelist[i].status == LS_INVALID)
            continue;
        describe_game(buf, gamelist[i].status, &gamelist[i].i);
        id = (gamelist[i].status == LS_IN_PROGRESS) ? 0 : gamelist[i].gameid;
        add_menu_item(items, size, icount, id, buf, 0, FALSE);
    }

    n = curses_display_menu(items, icount, "saved games", PICK_ONE,
                            PLHINT_ANYWHERE, pick);
    free(items);
    if (n <= 0)
        return;

    id = pick[0];

    create_game_windows();

    ret = playgame(id);

    destroy_game_windows();
    cleanup_messages();

    if (ret == GAME_OVER)
        show_topten(player.plname, settings.end_top, settings.end_around,
                    settings.end_own);
}

void
net_replay(void)
{
    /* TODO */
}
