/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

/*
** These are functions for generating internet addresses
** and internet names
**
**             Author : Dhrubajyoti Borthakur
**               28 July, 1994
*/

#ifndef INTERNET_H
#define INTERNET_H


#if !defined(SOCKET) && !defined(WIN32)
#define SOCKET int
#endif

/* maximum length of a machine name */
#define  MAXHOSTLEN     1024

#if defined(__cplusplus)
#include "MyString.h"
extern "C" {
#endif

/* Extract the port from a string of the form "<xx.xx.xx.xx:pppp>" */
int string_to_port( const char* addr );

/* Convert a hostname[:port] to sinful string */
char * hostname_to_string (const char * hostname, const int default_port );

const char *sock_to_string(SOCKET sockd);

/* Returns 1 if h1 and h2 are both hostnames which refer to the same
   host, 0 if they don't, and -1 on error. */
int same_host(const char *h1, const char *h2);

/* Returns TRUE if hostname belongs to the given domain, FALSE if not */
int host_in_domain(const char *host, const char *domain);

int is_ipv4_addr_implementation(const char *inbuf, struct in_addr *sin_addr,
			struct in_addr *mask_addr,int allow_wildcard);

int is_valid_sinful( const char *sinful );

/* returns the port integer from a given address, or -1 if there is
   none. */
int getPortFromAddr( const char* addr );

/* returns only the host information (not including '<', '>', ":2132",
   etc) from a given address.  the string returned is a newly
   allocated string which must be de-allocated with free(). */
char* getHostFromAddr( const char* addr );

/* Deprecated!  The claim id should no longer be used to get the st artd
   address, except for cases where this is required for backward
   compatibility.  The startd ClassAd should be fed into a DCDaemon
   object to get the address of the startd, because this handles
   private network addresses etc.
   returns the address from a given ClaimId.  the string returned is a
   newly allocated string which must be de-allocated with free().
*/
char* getAddrFromClaimId( const char* id );

// generates sinful string.
// it detects whether given ip address is IPv4 or IPv6.
int generate_sinful(char* buf, int len, const char* ip, int port);

#if defined(__cplusplus)
}
// MyString version is C++ only
MyString generate_sinful(const char* ip, int port);

/* Extract the IP address from a sinful string ("<xx.xx.xx.xx:pppp>")
   and return it in ipout as an ASCII string ("xx.xx.xx.xx") and returns
   true.  If the IP address is invalid, returns false and ipout is
   left unchanged. */
bool sinful_to_ipstr(const char * addr, MyString & ipout);
#endif

#endif /* INTERNET_H */
