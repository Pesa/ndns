/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2019, Regents of the University of California.
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

#include "logger.hpp"
#include "ndns-label.hpp"
#include "mgmt/management-tool.hpp"
#include "util/util.hpp"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

int
main(int argc, char* argv[])
{
  using std::string;
  using namespace ndn;

  string zoneStr;
  string db = ndns::getDefaultDatabaseFile();
  try {
    namespace po = boost::program_options;
    po::variables_map vm;

    po::options_description options("Generic Options");
    options.add_options()
      ("help,h",  "print this help message and exit")
      ("db,b",    po::value<std::string>(&db)->default_value(db), "path to NDNS database file")
      ;

    po::positional_options_description postion;
    postion.add("zone", 1);

    po::options_description cmdline_options;
    cmdline_options.add(options);

    po::parsed_options parsed =
      po::command_line_parser(argc, argv).options(cmdline_options).positional(postion).run();

    po::store(parsed, vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << "Usage: ndns-list-all-zones [-b db]" << std::endl;
      std::cout << options << std::endl;
      return 0;
    }
  }
  catch (const std::exception& ex) {
    std::cerr << "Parameter Error: " << ex.what() << std::endl;
    return 1;
  }

  try {
    KeyChain keyChain;
    ndn::ndns::ManagementTool tool(db, keyChain);
    tool.listAllZones(std::cout);
  }
  catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
}
