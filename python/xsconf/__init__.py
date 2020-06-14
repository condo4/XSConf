import os, os.path

class XSConf:

    def __init__(self, id):
        self.m_id = id
        self.m_map = {}
        self.m_map_arrays = {}

        home = os.environ["HOME"];

        # Parse: /etc/<id>.conf and /etc/<id>.conf.d/*.conf
        self._parsePath("/etc/", True);

        # Parse: ~/.config/<id>.conf
        self._parsePath(home + "/.config/");

        # Parse: /var/xsconf/<id>.conf
        self._parsePath("/var/xsconf/");

    def __getitem__(self, key):
        if key in self.m_map.keys():
            return self.m_map[key]
        return None

    def keys(self):
        return self.m_map.keys()

    def arrays(self):
        return self.m_map_arrays.keys()

    def array(self, key):
        if key in self.m_map_arrays.keys():
            return self.m_map_arrays[key]
        return None

    def _parsePath(self, path, dotd = False):
        count = 0;
        path += self.m_id + ".conf";
        count += self._parseFile(path);

        if(dotd):
            path += ".d";
            print("PARSE DIR %s"%path)
            if os.path.isdir(path):
                for p in os.listdir(path):
                    if p.endswith(".conf"):
                        count += self._parseFile(path + "/" + p);

        return count;

    def _parseFile(self, f):
        count = 0
        topic = None
        if not os.path.isfile(f):
            return 0
        print("PARSE %s"%f)
        with open(f) as fd:
            for line in [l.strip() for l in fd.readlines() if len(l.strip()) > 0 and l[0] != "#"]:
                if line[0] == "[": # Topic line
                    topic = line.strip("[]")
                    if topic == "General":
                        topic = None
                tab = line.split("=")
                if len(tab) != 2:
                    continue

                if topic:
                    id = topic + "/" + tab[0].strip()
                else:
                    id = tab[0].strip()

                if id.endswith("[]"): # Array
                    aid = id.strip("[]")
                    if not aid in self.m_map_arrays:
                        self.m_map_arrays[aid] = []
                    self.m_map_arrays[aid].append(tab[1].strip())
                else: # Normal item
                    self.m_map[id] = tab[1].strip()

                count += 1
        return count
