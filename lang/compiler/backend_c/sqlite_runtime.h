#pragma once
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// === SQLite Runtime for Detoprox ===
// Simplified helper layer for prepared statements (C99/C++17 compatible)

static int dpx_sqlite_open(const char* path, sqlite3** db) {
    if (sqlite3_open(path, db) != SQLITE_OK) {
        fprintf(stderr, "sqlite open fail: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    return 0;
}

static int dpx_sqlite_exec(sqlite3* db, const char* sql) {
    char* err = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "sqlite exec error: %s\n", err ? err : "?");
        sqlite3_free(err);
        return 1;
    }
    return 0;
}

// === Prepared statement helpers ===
static sqlite3_stmt* dpx_sqlite_prep(sqlite3* db, const char* sql) {
    sqlite3_stmt* st = 0;
    int rc = sqlite3_prepare_v2(db, sql, -1, &st, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "sqlite prep error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return st;
}

static void dpx_sqlite_bind_int(sqlite3_stmt* st, int idx, int v) {
    sqlite3_bind_int(st, idx, v);
}

static void dpx_sqlite_bind_text(sqlite3_stmt* st, int idx, const char* v) {
    sqlite3_bind_text(st, idx, v ? v : "", -1, SQLITE_TRANSIENT);
}

static void dpx_sqlite_bind_double(sqlite3_stmt* st, int idx, double v) {
    sqlite3_bind_double(st, idx, v);
}

static int dpx_sqlite_step_done(sqlite3_stmt* st) {
    int rc = sqlite3_step(st);
    return rc == SQLITE_DONE ? 0 : rc;
}

static int dpx_sqlite_step_row(sqlite3_stmt* st) {
    return sqlite3_step(st) == SQLITE_ROW;
}

static void dpx_sqlite_finalize(sqlite3_stmt* st) {
    if (st) sqlite3_finalize(st);
}

static long long dpx_sqlite_last_id(sqlite3* db) {
    return (long long)sqlite3_last_insert_rowid(db);
}

static int dpx_sqlite_column_count(sqlite3_stmt* st) {
    return sqlite3_column_count(st);
}

static const char* dpx_sqlite_column_name(sqlite3_stmt* st, int i) {
    return sqlite3_column_name(st, i);
}

static const char* dpx_sqlite_column_text(sqlite3_stmt* st, int i) {
    const unsigned char* t = sqlite3_column_text(st, i);
    return t ? (const char*)t : "";
}

static int dpx_sqlite_column_int(sqlite3_stmt* st, int i) {
    return sqlite3_column_int(st, i);
}

static double dpx_sqlite_column_double(sqlite3_stmt* st, int i) {
    return sqlite3_column_double(st, i);
}
