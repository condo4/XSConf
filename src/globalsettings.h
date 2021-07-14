#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <experimental/propagate_const>

/*
 * Source of configuration:
 * RO /etc/<id>.conf
 * RO /etc/<id>.conf.d/*.conf
 * RO ~/.config/<id>.conf
 * RW /var/globalsettings/<id>.conf
 */

class GlobalSettings
{
    class GlobalSettingsPrivate;
    std::experimental::propagate_const< // const-forwarding pointer wrapper
        std::unique_ptr<                // unique-ownership opaque pointer
            GlobalSettingsPrivate>> pImpl;      // to the forward-declared implementation class

public:
    GlobalSettings(std::string id);
    std::string operator[](const std::string&) const;
    std::vector<std::string> array(const std::string &id) const;
    std::vector<std::string> keys() const;
    std::vector<std::string> arrays() const;
};
