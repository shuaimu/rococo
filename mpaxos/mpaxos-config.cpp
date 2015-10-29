/*
 * mpaxos-config.cpp
 *
 * Created on: Aug 22, 2014
 * Author: loli
 */

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include <json/json.h>
#include <apr_time.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>

#include "mpaxos/mpaxos.h"
#include "commo.hpp"
#include "view.hpp"
#include "utils/hostname.h"
#include "utils/logger.h"



int flush__;

int mpaxos_config_load(const char *cf) {
  
  LOG_INFO("loading config file %s ...\n", cf);
	
	YAML::Node config;

    if (cf == NULL) {
		config = YAML::LoadFile("config/localhost-1.yaml");
    } else {
		config = YAML::LoadFile(cf);
    }
    if (config == NULL) {
        printf("cannot open config file: %s.\n", cf);
        return -1;
    }

	YAML::Node nodes = config["host"];

    for (std::size_t i = 0; i < nodes.size(); i++) {

		YAML::Node node = nodes[i];

		std::string name = node["name"].as<std::string>();
		std::string addr = node["addr"].as<std::string>();
        int port = node["port"].as<int>();

        // set a node in view
		mpaxos_add_node(name, addr, port);
        set_node(name.c_str(), addr.c_str(), port);
    }
    
	YAML::Node config_nodename = config["nodename"];
    if (config_nodename != NULL) {
		std::string nodename = config_nodename["nodename"].as<std::string>();
		mpaxos_set_me(nodename);
   //     mpaxos_config_set("nodename", nodename.c_str()); 
	}

    LOG_INFO("config file loaded\n");
    return 0;
}

int mpaxos_config_set(const char *key, const char *value) {
    if (strcmp(key, "nodename") == 0) {
		std::string value_str = value;
		mpaxos_set_me(value_str);
        set_nodename(value);                 
		host_info_t *my = mpaxos_whoami();
		LOG_INFO("whoami. name:%s,  addr: %s, port: %d", 
			my->name.c_str(), my->addr.c_str(), my->port);
	//	my->port = 2222;
		mpaxos_node_info(value_str);
    }
    return 0;
}

int mpaxos_config_get(const char *key, char** value) {
    return 0;
}

