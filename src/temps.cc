/*
 * Copyright (c) 2003-2009, John Wiegley.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of New Artisans LLC nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <system.hh>

#include "temps.h"

namespace ledger {

temporaries_t::~temporaries_t()
{
  if (post_temps) {
    foreach (post_t& post, *post_temps) {
      if (! post.xact->has_flags(ITEM_TEMP))
	post.xact->remove_post(&post);

      if (! post.account->has_flags(ACCOUNT_TEMP))
	post.account->remove_post(&post);
    }
  }
}

xact_t& temporaries_t::copy_xact(xact_t& origin)
{
  if (! xact_temps)
    xact_temps = std::list<xact_t>();

  xact_temps->push_back(origin);
  xact_t& temp(xact_temps->back());

  temp.add_flags(ITEM_TEMP);
  return temp;
}

xact_t& temporaries_t::create_xact()
{
  if (! xact_temps)
    xact_temps = std::list<xact_t>();

  xact_temps->push_back(xact_t());
  xact_t& temp(xact_temps->back());

  temp.add_flags(ITEM_TEMP);
  return temp;
}

post_t& temporaries_t::copy_post(post_t& origin, xact_t& xact,
				 account_t * account)
{
  if (! post_temps)
    post_temps = std::list<post_t>();

  post_temps->push_back(origin);
  post_t& temp(post_temps->back());

  if (account)
    temp.account = account;
  temp.add_flags(ITEM_TEMP);

  temp.account->add_post(&temp);
  xact.add_post(&temp);

  return temp;
}

post_t& temporaries_t::create_post(xact_t& xact, account_t * account)
{
  if (! post_temps)
    post_temps = std::list<post_t>();

  post_temps->push_back(post_t(account));
  post_t& temp(post_temps->back());

  temp.account = account;
  temp.add_flags(ITEM_TEMP);

  temp.account->add_post(&temp);
  xact.add_post(&temp);

  return temp;
}

account_t& temporaries_t::create_account(const string& name,
					 account_t *   parent)
{
  if (! acct_temps)
    acct_temps = std::list<account_t>();

  acct_temps->push_back(account_t(parent, name));
  account_t& temp(acct_temps->back());

  temp.add_flags(ACCOUNT_TEMP);
  return temp;
}

} // namespace ledger