#ifndef package
#define package
#include "types.hxx"
#include <set>
#include <list>
#include <map>
#include <algorithm>
//#include<bits/stdc++.h>

class Package {
    public:
        Package(ElementID ai = -1) : id_(przydziel_id(ai)) {}
        ElementID przydziel_id(ElementID ai)
        {
            if(ai == -1)
            {
                if(freed_IDs.empty())
                {
                    if(assigned_IDs.size() > 0)
                    {
                        ElementID ajdi = *max_element(assigned_IDs.begin(), assigned_IDs.end())+1;
                        assigned_IDs.insert(ajdi);
                        return ajdi;
                    }
                    ElementID ajdi = 1;
                    assigned_IDs.insert(ajdi);
                    return ajdi;
                }
                else
                {
                    ElementID ajdi = *min_element(freed_IDs.begin(), freed_IDs.end());
                    freed_IDs.erase(ajdi);
                    return ajdi;
                }
            }
            else
            {
                if(!assigned_IDs.count(ai))
                {
                    assigned_IDs.insert(ai);
                    return ai;
                }
                else
                {
                    return przydziel_id(-1);
                }
            }
        }
        ElementID get_id() const {return id_;}
        Package& operator=(const Package& other) = default;
        /*{
            if (this == &other)
                return *this;
            else
            {
                return *this;
            }
        }*/
        ~Package()
        {
            assigned_IDs.erase(id_);
            freed_IDs.insert(id_);
        }
    private:
        inline static std::set<ElementID> assigned_IDs;
        inline static std::set<ElementID> freed_IDs;
        ElementID id_;
};
extern std::set<ElementID> assigned_IDs;
extern std::set<ElementID> freed_IDs;
#endif
