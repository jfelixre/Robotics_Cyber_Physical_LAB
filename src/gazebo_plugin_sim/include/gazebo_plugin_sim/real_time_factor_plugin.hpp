#ifndef GAZEBO_PLUGINS__REAL_TIME_FACTOR_PLUGIN_HPP_
#define GAZEBO_PLUGINS__REAL_TIME_FACTOR_PLUGIN_HPP_

#include <gazebo/common/Plugin.hh>

#include <memory>

namespace gazebo_plugin_sim
{
class RealTimeFactorPluginPrivate;

class RealTimeFactorPlugin : public gazebo::WorldPlugin
{
public:
  /// Constructor
  RealTimeFactorPlugin();

  /// Destructor
  ~RealTimeFactorPlugin();

protected:
  // Documentation inherited
  void Load(gazebo::physics::WorldPtr _world, sdf::ElementPtr _sdf) override;

  // Documentation inherited
  void Reset() override;

private:
  /// Private data pointer
  std::unique_ptr<RealTimeFactorPluginPrivate> impl_;
};
}  // namespace gazebo_plugin_sim

#endif  // GAZEBO_PLUGINS__REAL_TIME_FACTOR_PLUGIN_HPP_