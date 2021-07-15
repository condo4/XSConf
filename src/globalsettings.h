#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <string>
#include <map>
#include <vector>
#include <memory>

/*
 * Source of configuration:
 * RO /etc/<id>.conf
 * RO /etc/<id>.conf.d/*.conf
 * RO ~/.config/<id>.conf
 * RW /var/globalsettings/<id>.conf
 */

class GlobalSettingsPrivate;

class GlobalSettings
{
public:
    explicit GlobalSettings(std::string id);
    virtual ~GlobalSettings();

    std::string operator[](const std::string&) const;
    std::vector<std::string> array(const std::string &id) const;
    std::vector<std::string> keys() const;
    std::vector<std::string> arrays() const;

private:
    std::unique_ptr<GlobalSettingsPrivate> m_ptr;
};

#endif // GLOBALSETTINGS_H
