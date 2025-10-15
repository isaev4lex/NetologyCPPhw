#include "stopwatch.h"

Stopwatch::Stopwatch(QObject* parent) : QObject(parent)
{
    setUpdateIntervalMs(100); // millisekundi
    connect(&timer_, &QTimer::timeout, this, &Stopwatch::tick);
}

void Stopwatch::setUpdateIntervalMs(int ms)
{
    timer_.setInterval(ms < 10 ? 10 : ms);
}

qint64 Stopwatch::nowTotalMs() const
{
    if (!running_) return baseAccumMs_;
    return baseAccumMs_ + coarse_.elapsed();
}

qint64 Stopwatch::elapsedMs() const
{
    return clampNonNeg(nowTotalMs());
}

void Stopwatch::start()
{
    if (running_) return;
    running_ = true;
    coarse_.restart();
    timer_.start();
    emit runningChanged(true);
}

void Stopwatch::stop()
{
    if (!running_) return;

    baseAccumMs_ += coarse_.elapsed();
    running_ = false;
    timer_.stop();
    emit runningChanged(false);
    emit elapsedChanged(elapsedMs());
}

void Stopwatch::reset()
{
    timer_.stop();
    running_ = false;
    baseAccumMs_   = 0;
    lastLapMarkMs_ = 0;
    lapIndex_      = 0;
    emit runningChanged(false);
    emit elapsedChanged(0);
    emit resetDone();
}

void Stopwatch::tick()
{
    emit elapsedChanged(elapsedMs());
}

void Stopwatch::markLap()
{
    if (!running_) return;
    const qint64 now = nowTotalMs();
    qint64 lap = now - lastLapMarkMs_;
    if (lap < 0) lap = 0;
    ++lapIndex_;
    lastLapMarkMs_ = now;
    emit lapRecorded(lapIndex_, lap);
}
