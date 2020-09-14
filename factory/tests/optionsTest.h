/*
 * optionsTest.h
 * monkey around with boost program_options
 *
 */
#pragma once

#include "gtest/gtest.h"
#include "include/singleton.h"
#include "boost/program_options.hpp"

namespace {
  namespace BPO = boost::program_options;
  
  class options: public Factory::Singleton<options>
  {
  public:
    options(const options&) = delete; // no copy
    
    void Parse(int argc, const char * const * argv)
    {
      BPO:: variables_map vm;
      auto parsedArgs = BPO::parse_command_line(argc, argv, desc);
      std::cout << help << std::endl;
      if (help)
	std::cout << desc << std::endl;
      if (boolArg)
	std::cout << "verbosity is on" << std::endl;
      if (intArg > 0)
	std::cout << "Compression Level " << intArg << std::endl;
      
    }
    
  private:
    const std::string name {"options"};
    BPO::options_description desc;
    bool help;
    bool boolArg;
    int intArg;
    
    friend Factory::Singleton<options>;
    options():
      desc("testing options parsing")
    {
      std::cout << name << " was created by Singleton" << std::endl;
      desc.add_options()
	("help",
	 BPO::value<bool>(&help)->default_value(false),
	 "help menu")
	("compression",
	 BPO::value<int>(&intArg)->default_value(0),
	 "compression level")
	("verbosity",
	 BPO::value<bool>(&boolArg)->default_value(false),
	 "turn on verbosity")

	;
    }
  
  };

  
  TEST(SimpleTest, options) {
    auto oP = options::Create();
    int argc = 1;
    const char* const  argv[] {"--help"};
    oP->Parse(argc, argv);
  }


}  // namespace
