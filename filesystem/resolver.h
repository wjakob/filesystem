// Copyright 2015 Wenzel Jakob. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if !defined(__FILESYSTEM_RESOLVER_H)
#define __FILESYSTEM_RESOLVER_H

#include "path.h"

namespace filesystem {

/**
 * \brief Simple class for manipulating paths on Linux/Windows/Mac OS
 *
 * This class is just a temporary workaround to avoid the heavy boost
 * dependency until boost::filesystem is integrated into the standard template
 * library at some point in the future.
 */
class resolver {
public:
    typedef std::vector<path>::iterator iterator;
    typedef std::vector<path>::const_iterator const_iterator;

    resolver() {
        m_paths.push_back(path::getcwd());
    }

    size_t size() const { return m_paths.size(); }

    iterator begin() { return m_paths.begin(); }
    iterator end()   { return m_paths.end(); }

    const_iterator begin() const { return m_paths.begin(); }
    const_iterator end()   const { return m_paths.end(); }

    void erase(iterator it) { m_paths.erase(it); }

    void prepend(const path &path) { m_paths.insert(m_paths.begin(), path); }
    void append(const path &path) { m_paths.push_back(path); }


    path resolve(const path &value) const {
        for (const_iterator it = m_paths.begin(); it != m_paths.end(); ++it) {
            path combined = *it / value;
            if (combined.exists())
                return combined;
        }
        return path();
    }

private:
    std::vector<path> m_paths;
};

}; /* namespace filesystem */

#endif /* __FILESYSTEM_RESOLVER_H */
