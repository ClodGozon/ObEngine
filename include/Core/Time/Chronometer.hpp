#pragma once

#include <chrono>

#include <Time/TimeUtils.hpp>

namespace obe::Time
{
    /**
     * \brief A Chronometer class to measure time
     * \bind{Chronometer}
     */
    class Chronometer
    {
    private:
        std::chrono::high_resolution_clock::time_point m_start;
        bool m_started = false;
        unsigned long long int m_limit = 0;

    public:
        /**
         * \brief Starts the Chronometer
         */
        void start();
        /**
         * \brief Stops the Chronometer
         */
        void stop();
        /**
         * \brief Resets the Chronometer
         */
        void reset();
        /**
         * \brief Get Time elapsed since the Chronometer started
         * \return The amount of milliseconds elapsed since the Chronometer
         * started
         */
        TimeUnit getTime() const;
        /**
         * \brief Defines a limit to the Chronometer
         * \param limit The amount of milliseconds before the limit is exceeded
         */
        void setLimit(TimeUnit limit);
        TimeUnit getLimit() const;
        /**
         * \brief Check if the defined limit has been exceeded
         * \return true if the limit has been exceeded, false otherwise.
         */
        bool over() const;
    };
} // namespace obe::Time