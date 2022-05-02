/**
 * @file radio_data_plane.h
 * @brief Declaration file for the RadioDataPlane Class
 */
#ifndef RADIO_DATA_PLANE_H_
#define RADIO_DATA_PLANE_H_

#include <complex>
#include <vector>

#include "SoapySDR/Device.hpp"
#include "config.h"
#include "radio.h"

//Abstract class defination for the Radio data plane connection
class RadioDataPlane {
 public:
  enum DataPlaneType { SoapyStream, LinuxSocket };
  static std::unique_ptr<RadioDataPlane> Create(DataPlaneType type);

  enum Mode { kModeUninit, kModeShutdown, kModeDeactive, kModeActive };

  virtual ~RadioDataPlane();
  RadioDataPlane(RadioDataPlane&&) noexcept = delete;
  explicit RadioDataPlane(const RadioDataPlane&) = delete;

  virtual void Init(Radio* radio, const Config* cfg) = 0;
  virtual void Setup() = 0;
  virtual void Activate() = 0;
  virtual void Deactivate() = 0;
  virtual void Close() = 0;

  virtual int Rx(std::vector<std::vector<std::complex<int16_t>>>& rx_data,
                 size_t rx_size, Radio::RxFlags rx_flags,
                 long long& rx_time_ns) = 0;
  virtual int Rx(std::vector<std::vector<std::complex<int16_t>>*>& rx_buffs,
                 size_t rx_size, Radio::RxFlags rx_flags,
                 long long& rx_time_ns) = 0;
  virtual int Rx(std::vector<void*>& rx_locations, size_t rx_size,
                 Radio::RxFlags rx_flags, long long& rx_time_ns) = 0;

  virtual void Flush() = 0;

 protected:
  RadioDataPlane();

  virtual void Setup(const SoapySDR::Kwargs& args);
  inline const Config* Configuration() const { return cfg_; }
  inline const Mode& CheckMode() const { return mode_; }

  Radio* radio_;
  SoapySDR::Stream* remote_stream_;

 private:
  enum Mode mode_;
  //Should try to remove cfg_
  const Config* cfg_;
};
#endif  // RADIO_DATA_PLANE_H_