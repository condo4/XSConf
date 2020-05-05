#include "xsconf.h"
#include <fstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdexcept>

static inline std::vector<std::string> split(const std::string& s, char seperator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

static inline std::string trim(const std::string& src, const std::string &remove = " \n\r\t")
{
    std::string str(src);
    str.erase(0, str.find_first_not_of(remove));       //prefixing spaces
    str.erase(str.find_last_not_of(remove)+1);         //surfixing spaces
    return str;
}

static inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static inline std::vector<std::string> listdir(std::string dir)
{
    std::vector<std::string> files;
    DIR *dp;
    struct dirent *dirp;
    dp  = opendir(dir.c_str());

    if(dp)
    {
        while ((dirp = readdir(dp)) != nullptr)
        {
            files.push_back(std::string(dirp->d_name));
        }
        closedir(dp);
    }

    return files;
}

// Private Implementation
class XSConf::XSConfPrivate
{
    std::string m_id;
    std::map<std::string, std::string> m_map;
    std::map<std::string, std::vector<std::string>> m_map_arrays;

    int _parsePath(std::string path, bool dotd = false)
    {
        struct stat st;
        int count = 0;
        path += m_id + ".conf";
        count += _parseFile(path);

        if(dotd)
        {
            path += ".d";
            if(stat(path.c_str(),&st) == 0 && ((st.st_mode & S_IFDIR) != 0))
            {
                for(auto& p: listdir(path))
                {
                    if(ends_with(path + "/" + p, ".conf"))
                    {
                        count += _parseFile(path + "/" + p);
                    }
                }
            }
        }
        return count;
    }

    int _parseFile(std::string file)
    {
        std::string line;
        std::string url;
        std::string topic; /* [General] is the root topic */
        int count = 0;

        if(access(file.c_str(), R_OK) != 0)
        {
            return 0;
        }

        std::ifstream infile(file);
        while (std::getline(infile, line))
        {
            line = trim(line);
            if(line[0] == '#')
                continue;

            if(line[0] == '[') /* Topic line */
            {
                topic = trim(line, "[]");
                if(topic == "General")
                    topic.clear();
                continue;
            }

            std::vector<std::string> lineconf = split(line, '=');
            if(lineconf.size() != 2)
                continue;

            std::string id = (topic.size())?(topic + "/" + lineconf[0]):(lineconf[0]);
            if(lineconf[0][lineconf[0].size() - 1] == ']')
            {
                if(m_map_arrays.count(trim(id, "[]")) == 0)
                {
                    m_map_arrays[trim(id, "[]")] = {};
                }
                m_map_arrays[trim(id, "[]")].push_back(lineconf[1]);
            }
            else
            {
                m_map[id] = lineconf[1];
            }

            ++count;
        }
        return count;
    }

public:
    XSConfPrivate(std::string id)
        : m_id(id)
    {
        std::string home = std::getenv("HOME");

        // Parse: /etc/<id>.conf and /etc/<id>.conf.d/*.conf
        _parsePath("/etc/", true);

        // Parse: ~/.config/<id>.conf
        _parsePath(home + "/.config/");

        // Parse: /var/xsconf/<id>.conf
        _parsePath("/var/xsconf/");
    }

    std::string get(const XSConf &x, const std::string& id) const
    {
        if(!m_map.count(id))
            throw std::out_of_range(id + " don't exists");
        return m_map.at(id);
    }

    std::vector<std::string> array(const XSConf &x, const std::string &id) const
    {
        if(!m_map_arrays.count(id))
            throw std::out_of_range(id + " don't exists");
        return m_map_arrays.at(id);
    }

    std::vector<std::string> keys(const XSConf &x) const
    {
        std::vector<std::string> keys;
        for(std::map<std::string,std::string>::const_iterator it = m_map.begin(); it != m_map.end(); ++it)
        {
            keys.push_back(it->first);
        }
        return keys;
    }

    std::vector<std::string> arrays(const XSConf &x) const
    {
        std::vector<std::string> keys_arrays;
        for(std::map<std::string,std::vector<std::string>>::const_iterator it = m_map_arrays.begin(); it != m_map_arrays.end(); ++it)
        {
            keys_arrays.push_back(it->first);
        }
        return keys_arrays;
    }

};

XSConf::XSConf(std::string id)
    : pImpl{std::make_unique<XSConfPrivate>(id)}
    {}

std::string XSConf::operator[](const std::string &id) const { return pImpl->get(*this, id); }
std::vector<std::string> XSConf::array(const std::string &id) const { return pImpl->array(*this, id); }
std::vector<std::string> XSConf::keys() const { return pImpl->keys(*this); }
std::vector<std::string> XSConf::arrays() const { return pImpl->arrays(*this); }
