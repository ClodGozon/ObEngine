#include <Bindings/Transform/Transform.hpp>

#include <Transform/Matrix2D.hpp>
#include <Transform/Movable.hpp>
#include <Transform/Polygon.hpp>
#include <Transform/Rect.hpp>
#include <Transform/Referential.hpp>
#include <Transform/UnitBasedObject.hpp>
#include <Transform/UnitVector.hpp>
#include <Transform/Units.hpp>

#include <sol/sol.hpp>

namespace obe::Transform::Bindings
{
    void LoadEnumUnits(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        TransformNamespace.new_enum<obe::Transform::Units>("Units",
            { { "ViewPercentage", obe::Transform::Units::ViewPercentage },
                { "ViewPixels", obe::Transform::Units::ViewPixels },
                { "ViewUnits", obe::Transform::Units::ViewUnits },
                { "ScenePixels", obe::Transform::Units::ScenePixels },
                { "SceneUnits", obe::Transform::Units::SceneUnits } });
    }
    void LoadClassMatrix2D(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::Matrix2D> bindMatrix2D
            = TransformNamespace.new_usertype<obe::Transform::Matrix2D>("Matrix2D",
                sol::call_constructor,
                sol::constructors<obe::Transform::Matrix2D(std::array<double, 4>)>());
        bindMatrix2D["product"] = &obe::Transform::Matrix2D::product;
    }
    void LoadClassMovable(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::Movable> bindMovable
            = TransformNamespace.new_usertype<obe::Transform::Movable>(
                "Movable", sol::call_constructor, sol::default_constructor);
        bindMovable["setPosition"] = &obe::Transform::Movable::setPosition;
        bindMovable["move"] = &obe::Transform::Movable::move;
        bindMovable["getPosition"] = &obe::Transform::Movable::getPosition;
    }
    void LoadClassPolygon(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::Polygon> bindPolygon
            = TransformNamespace.new_usertype<obe::Transform::Polygon>("Polygon",
                sol::call_constructor, sol::default_constructor, sol::base_classes,
                sol::bases<obe::Transform::UnitBasedObject, obe::Transform::Movable>());
        bindPolygon["addPoint"] = sol::overload(
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position)
                -> void { return self->addPoint(position); },
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position,
                int pointIndex) -> void { return self->addPoint(position, pointIndex); });
        bindPolygon["findClosestSegment"] = &obe::Transform::Polygon::findClosestSegment;
        bindPolygon["findClosestPoint"] = sol::overload(
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position)
                -> obe::Transform::PolygonPoint& {
                return self->findClosestPoint(position);
            },
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position,
                bool neighbor) -> obe::Transform::PolygonPoint& {
                return self->findClosestPoint(position, neighbor);
            },
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position,
                bool neighbor,
                const std::vector<obe::Transform::point_index_t>& excludedPoints)
                -> obe::Transform::PolygonPoint& {
                return self->findClosestPoint(position, neighbor, excludedPoints);
            });
        bindPolygon["getAllPoints"] = &obe::Transform::Polygon::getAllPoints;
        bindPolygon["getCentroid"] = &obe::Transform::Polygon::getCentroid;
        bindPolygon["getPointsAmount"] = &obe::Transform::Polygon::getPointsAmount;
        bindPolygon["getPosition"] = &obe::Transform::Polygon::getPosition;
        bindPolygon["getRotation"] = &obe::Transform::Polygon::getRotation;
        bindPolygon["getSegment"] = &obe::Transform::Polygon::getSegment;
        bindPolygon["getSegmentContainingPoint"] = sol::overload(
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position)
                -> std::optional<obe::Transform::PolygonSegment> {
                return self->getSegmentContainingPoint(position);
            },
            [](obe::Transform::Polygon* self, const obe::Transform::UnitVector& position,
                double tolerance) -> std::optional<obe::Transform::PolygonSegment> {
                return self->getSegmentContainingPoint(position, tolerance);
            });
        bindPolygon["isCentroidAroundPosition"]
            = &obe::Transform::Polygon::isCentroidAroundPosition;
        bindPolygon["getPointAroundPosition"]
            = &obe::Transform::Polygon::getPointAroundPosition;
        bindPolygon["move"] = &obe::Transform::Polygon::move;
        bindPolygon["rotate"] = &obe::Transform::Polygon::rotate;
        bindPolygon["setPosition"] = &obe::Transform::Polygon::setPosition;
        bindPolygon["setRotation"] = &obe::Transform::Polygon::setRotation;
        bindPolygon["setPositionFromCentroid"]
            = &obe::Transform::Polygon::setPositionFromCentroid;
        bindPolygon["operator[]"] = &obe::Transform::Polygon::operator[];
        bindPolygon["get"] = &obe::Transform::Polygon::get;
        bindPolygon["getBoundingBox"] = &obe::Transform::Polygon::getBoundingBox;
    }
    void LoadClassPolygonPoint(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::PolygonPoint> bindPolygonPoint
            = TransformNamespace.new_usertype<obe::Transform::PolygonPoint>(
                "PolygonPoint", sol::call_constructor,
                sol::constructors<obe::Transform::PolygonPoint(
                                      obe::Transform::Polygon&, unsigned int),
                    obe::Transform::PolygonPoint(obe::Transform::Polygon&, unsigned int,
                        const obe::Transform::UnitVector&)>(),
                sol::base_classes, sol::bases<obe::Transform::UnitVector>());
        bindPolygonPoint["remove"] = &obe::Transform::PolygonPoint::remove;
        bindPolygonPoint["distance"] = &obe::Transform::PolygonPoint::distance;
        bindPolygonPoint["getRelativePosition"]
            = &obe::Transform::PolygonPoint::getRelativePosition;
        bindPolygonPoint["setRelativePosition"]
            = &obe::Transform::PolygonPoint::setRelativePosition;
        bindPolygonPoint["move"] = &obe::Transform::PolygonPoint::move;
        bindPolygonPoint["index"] = sol::property(
            [](obe::Transform::PolygonPoint* self) -> const point_index_t& {
                return self->index;
            });
    }
    void LoadClassPolygonSegment(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::PolygonSegment> bindPolygonSegment
            = TransformNamespace.new_usertype<obe::Transform::PolygonSegment>(
                "PolygonSegment", sol::call_constructor,
                sol::constructors<obe::Transform::PolygonSegment(
                    const obe::Transform::PolygonPoint&,
                    const obe::Transform::PolygonPoint&)>());
        bindPolygonSegment["getAngle"] = &obe::Transform::PolygonSegment::getAngle;
        bindPolygonSegment["getLength"] = &obe::Transform::PolygonSegment::getLength;
        bindPolygonSegment["first"] = sol::property(
            [](obe::Transform::PolygonSegment* self) -> const PolygonPoint& {
                return self->first;
            });
        bindPolygonSegment["second"] = sol::property(
            [](obe::Transform::PolygonSegment* self) -> const PolygonPoint& {
                return self->second;
            });
    }
    void LoadClassRect(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::Rect> bindRect
            = TransformNamespace.new_usertype<obe::Transform::Rect>("Rect",
                sol::call_constructor,
                sol::constructors<obe::Transform::Rect(),
                    obe::Transform::Rect(const obe::Transform::UnitVector&,
                        const obe::Transform::UnitVector&)>(),
                sol::base_classes, sol::bases<obe::Transform::Movable>());
        bindRect["transformRef"] = &obe::Transform::Rect::transformRef;
        bindRect["setPosition"] = sol::overload(
            static_cast<void (obe::Transform::Rect::*)(
                const obe::Transform::UnitVector&)>(&obe::Transform::Rect::setPosition),
            static_cast<void (obe::Transform::Rect::*)(const obe::Transform::UnitVector&,
                obe::Transform::Referential)>(&obe::Transform::Rect::setPosition));
        bindRect["getPosition"] = sol::overload(
            static_cast<obe::Transform::UnitVector (obe::Transform::Rect::*)() const>(
                &obe::Transform::Rect::getPosition),
            static_cast<obe::Transform::UnitVector (obe::Transform::Rect::*)(
                obe::Transform::Referential) const>(&obe::Transform::Rect::getPosition));
        bindRect["move"] = &obe::Transform::Rect::move;
        bindRect["setPointPosition"] = sol::overload(
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& position)
                -> void { return self->setPointPosition(position); },
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& position,
                obe::Transform::Referential ref) -> void {
                return self->setPointPosition(position, ref);
            });
        bindRect["movePoint"] = sol::overload(
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& position)
                -> void { return self->movePoint(position); },
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& position,
                obe::Transform::Referential ref) -> void {
                return self->movePoint(position, ref);
            });
        bindRect["setSize"] = sol::overload(
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& size)
                -> void { return self->setSize(size); },
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& size,
                obe::Transform::Referential ref) -> void {
                return self->setSize(size, ref);
            });
        bindRect["scale"] = sol::overload(
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& size)
                -> void { return self->scale(size); },
            [](obe::Transform::Rect* self, const obe::Transform::UnitVector& size,
                obe::Transform::Referential ref) -> void {
                return self->scale(size, ref);
            });
        bindRect["getSize"] = &obe::Transform::Rect::getSize;
        bindRect["getScaleFactor"] = &obe::Transform::Rect::getScaleFactor;
        bindRect["getRotation"] = &obe::Transform::Rect::getRotation;
        bindRect["setRotation"] = &obe::Transform::Rect::setRotation;
        bindRect["rotate"] = &obe::Transform::Rect::rotate;
        bindRect["draw"] = &obe::Transform::Rect::draw;
    }
    void LoadClassReferential(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::Referential> bindReferential
            = TransformNamespace.new_usertype<obe::Transform::Referential>("Referential",
                sol::call_constructor,
                sol::constructors<obe::Transform::Referential(),
                    obe::Transform::Referential(double, double)>());
        bindReferential[sol::meta_function::equal_to]
            = &obe::Transform::Referential::operator==;
        bindReferential["FromString"] = &obe::Transform::Referential::FromString;
        bindReferential["TopLeft"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::TopLeft;
        });
        bindReferential["Top"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::Top;
        });
        bindReferential["TopRight"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::TopRight;
        });
        bindReferential["Left"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::Left;
        });
        bindReferential["Center"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::Center;
        });
        bindReferential["Right"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::Right;
        });
        bindReferential["BottomLeft"]
            = sol::property([]() -> obe::Transform::Referential {
                  return obe::Transform::Referential::BottomLeft;
              });
        bindReferential["Bottom"] = sol::property([]() -> obe::Transform::Referential {
            return obe::Transform::Referential::Bottom;
        });
        bindReferential["BottomRight"]
            = sol::property([]() -> obe::Transform::Referential {
                  return obe::Transform::Referential::BottomRight;
              });
    }
    void LoadClassUnitBasedObject(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::UnitBasedObject> bindUnitBasedObject
            = TransformNamespace.new_usertype<obe::Transform::UnitBasedObject>(
                "UnitBasedObject");
        bindUnitBasedObject["setWorkingUnit"]
            = &obe::Transform::UnitBasedObject::setWorkingUnit;
        bindUnitBasedObject["getWorkingUnit"]
            = &obe::Transform::UnitBasedObject::getWorkingUnit;
    }
    void LoadClassUnitVector(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        sol::usertype<obe::Transform::UnitVector> bindUnitVector
            = TransformNamespace.new_usertype<obe::Transform::UnitVector>("UnitVector",
                sol::call_constructor,
                sol::constructors<obe::Transform::UnitVector(),
                    obe::Transform::UnitVector(obe::Transform::Units),
                    obe::Transform::UnitVector(double, double),
                    obe::Transform::UnitVector(double, double, obe::Transform::Units)>());
        bindUnitVector["set"] = sol::overload(
            static_cast<void (obe::Transform::UnitVector::*)(
                const obe::Transform::UnitVector&)>(&obe::Transform::UnitVector::set),
            static_cast<void (obe::Transform::UnitVector::*)(double, double)>(
                &obe::Transform::UnitVector::set));
        bindUnitVector["add"] = sol::overload(
            static_cast<void (obe::Transform::UnitVector::*)(
                const obe::Transform::UnitVector&)>(&obe::Transform::UnitVector::add),
            static_cast<void (obe::Transform::UnitVector::*)(double, double)>(
                &obe::Transform::UnitVector::add));
        bindUnitVector[sol::meta_function::addition] = sol::overload(
            static_cast<obe::Transform::UnitVector (obe::Transform::UnitVector::*)(
                const obe::Transform::UnitVector&) const>(
                &obe::Transform::UnitVector::operator+),
            static_cast<obe::Transform::UnitVector (obe::Transform::UnitVector::*)(double)
                    const>(&obe::Transform::UnitVector::operator+));
        bindUnitVector["x"] = &obe::Transform::UnitVector::x;
        bindUnitVector["y"] = &obe::Transform::UnitVector::y;
        bindUnitVector["unit"] = &obe::Transform::UnitVector::unit;
        bindUnitVector["to"]
            = static_cast<obe::Transform::UnitVector (obe::Transform::UnitVector::*)(
                obe::Transform::Units) const>(&obe::Transform::UnitVector::to);
    }
    void LoadFunctionStringToUnits(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        TransformNamespace.set_function("stringToUnits", obe::Transform::stringToUnits);
    }
    void LoadFunctionUnitsToString(sol::state_view state)
    {
        sol::table TransformNamespace = state["obe"]["Transform"].get<sol::table>();
        TransformNamespace.set_function("unitsToString", obe::Transform::unitsToString);
    }
};