/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014, Regents of the University of California.
 *
 * This file is part of NDNS (Named Data Networking Domain Name Service).
 * See AUTHORS.md for complete list of NDNS authors and contributors.
 *
 * NDNS is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NDNS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NDNS, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "clients/response.hpp"
#include <ndn-cxx/security/key-chain.hpp>
#include "../../boost-test.hpp"

namespace ndn {
namespace ndns {
namespace tests {



BOOST_AUTO_TEST_SUITE(Response)

BOOST_AUTO_TEST_CASE(Basic)
{
  KeyChain keyChain;
  Name hint;
  Name zone("/net");
  name::Component qType = ndns::label::NDNS_ITERATIVE_QUERY;


  ndns::Response r(zone, qType);
  r.setRrLabel(Name("/ndnsim/www"));
  r.setRrType(label::CERT_RR_TYPE);
  r.setNdnsType(NDNS_RAW);
  r.setFreshnessPeriod(time::seconds(4000));

  BOOST_CHECK_EQUAL(r.getFreshnessPeriod(), time::seconds(4000));
  BOOST_CHECK_EQUAL(r.getRrType(), label::CERT_RR_TYPE);
  BOOST_CHECK_EQUAL(r.getNdnsType(), NDNS_RAW);
  BOOST_CHECK_EQUAL(r.getZone(), zone);
  BOOST_CHECK_EQUAL(r.getQueryType(), qType);

  const std::string DATA1("some fake content");
  r.setAppContent(dataBlock(ndn::tlv::Content, DATA1.c_str(), DATA1.size()));

  //const Block& block = r.wireEncode();
  shared_ptr<Data> data = r.toData();
  // keyChain.sign(*data);

  ndns::Response r2;
  BOOST_CHECK_EQUAL(r2.fromData(hint, zone, *data), true);
  BOOST_CHECK_EQUAL(r, r2);

  ndns::Response r4(zone, qType);
  r4.setRrLabel(Name("/ndnsim/www"));
  r4.setRrType(label::TXT_RR_TYPE);
  r4.setNdnsType(NDNS_RESP);

  std::string str = "Just try it";
  Block s = dataBlock(ndns::tlv::RrData, str.c_str(), str.size());
  r4.addRr(s);
  str = "Go to Hell";
  // Block s2 = dataBlock(ndns::tlv::RrData, str.c_str(), str.size());
  r4.addRr(str);

  BOOST_CHECK_NE(r2, r4);

  data = r4.toData();
  // keyChain.sign(*data);

  ndns::Response r5(zone, qType);

  BOOST_CHECK_EQUAL(r5.fromData(hint, zone, *data), true);
  BOOST_CHECK_EQUAL(r4, r5);
 }

BOOST_AUTO_TEST_SUITE_END()

} // namespace tests
} // namespace ndns
} // namespace ndn