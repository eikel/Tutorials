#include "E_BoundingBox2D.h"
#include <E_Geometry/E_Vec2.h>

namespace E_MinSG {
namespace E_MyExtension {
//! returns an instance to an EScript::Type
EScript::Type * E_BoundingBox2D::getTypeObject() {
    // you want to have this static in order to return always the same instance
	// E_BoundingBox2D ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_BoundingBox2D::init(EScript::Namespace& lib) {
	EScript::Type* typeObject = getTypeObject();
	// first define the class type in EScript
	declareConstant(&lib, getClassName(), typeObject);
	
	// We're lazy and we only want to write BoundingBox2D instead of the fully qualified name
	using MinSG::MyExtension::BoundingBox2D;
	// now you should define all functions for your new type
	// this is done by several macros
	//! BoundingBox2D new BoundingBox2D([otherBB | min2D, max2D | minX, minY, maxX, maxY])
	ES_CONSTRUCTOR(typeObject, 0, 4,{ // typeObject, minimal parameter count, maximal parameter count, function body
        // Depending on the parameter count, we want to use an other constructor
        // Here we can return a new BoundingBox2D, because the corresponding macro can convert it to an E_BoundingBox2D
        // Of course we could also return a new E_BoundingBox2D directly
        switch(parameter.count()) {
            // parameterless constructor: BoundingBox2D()
            case 0: return EScript::create(new BoundingBox2D());
            // as already mentioned we could also do this:
            // case 0: return new E_BoundingBox2D();
            
            // Copy constructor: BoundingBox2(BoundingBox2D otherBB)
            case 1: return EScript::create(new BoundingBox2D( parameter[0].to<BoundingBox2D*>(rt) ));
            // Constructor using the extreme points: BoundingBox2D(Vec2 min, Vec2 max)
            case 2: return EScript::create(new BoundingBox2D( parameter[0].to<Vec2*>(rt), parameter[1].to<Vec2*>(rt) ));
            // float constructor: BoundingBox2D(float minX, float minY, float maxX, float maxY)
            case 4: return EScript::create(new BoundingBox2D( parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toFloat() ));
            // Something went wrong!
            default:
                rt.warn("new BoundingBox2D(): Wrong parameter count!");
                return EScript::create(new BoundingBox2D());
        }
	})
	
	// short version of the macro, the function body will be converted to this: return EScript::value( thisObj->getMinX() );
	ES_MFUN(typeObject, BoundingBox2D, "getMinX", 0, 0, thisObj->getMinX())
	/* This could be also written in a long version:
	ES_MFUNCTION(typeObject, BoundingBox2D, "getMinX", 0, 0, {
        return thisObj->getMinX();
    })
	*/
	ES_MFUN(typeObject, BoundingBox2D, "setMinX", 1, 1, (thisObj->setMinX(parameter[0].toFloat()), thisEObj) )
	/* This could be also written in a long version:
	ES_MFUNCTION(typeObject, BoundingBox2D, "setMinX", 1, 1, {
        thisObj->setMinX(parameter[0].toFloat());
        return thisObj;
    })
	*/
	ES_MFUN(typeObject, BoundingBox2D, "getMinY", 0, 0, thisObj->getMinY())
	//ES_MFUN(typeObject, BoundingBox2D, "setMinY", 1, 1, (thisObj->setMinY(parameter[0].toFloat()), thisEObj) )
	ES_MFUNCTION(typeObject, BoundingBox2D, "setMinY", 1, 1, {
        return EScript::value((thisObj->setMinY(parameter[0].toFloat()), thisEObj));
    })
	
	ES_MFUN(typeObject, BoundingBox2D, "getMaxX", 0, 0, thisObj->getMaxX())
	ES_MFUN(typeObject, BoundingBox2D, "setMaxX", 1, 1, (thisObj->setMaxX(parameter[0].toFloat()), thisEObj) )
	ES_MFUN(typeObject, BoundingBox2D, "getMaxY", 0, 0, thisObj->getMaxY())
	ES_MFUN(typeObject, BoundingBox2D, "setMaxY", 1, 1, (thisObj->setMaxY(parameter[0].toFloat()), thisEObj) )
	
	ES_MFUN(typeObject, BoundingBox2D, "getWidth", 0, 0, thisObj->getWidth())
	ES_MFUN(typeObject, BoundingBox2D, "getHeight", 0, 0, thisObj->getHeight())
	ES_MFUN(typeObject, BoundingBox2D, "getArea", 0, 0, thisObj->getArea())
	
	ES_MFUN(typeObject, BoundingBox2D, "getMin", 0, 0, EScript::create(thisObj->getMin()))
	ES_MFUN(typeObject, BoundingBox2D, "setMin", 1, 1, (thisObj->setMin(parameter[0].to<Vec2>(rt)), thisEObj) )
	ES_MFUN(typeObject, BoundingBox2D, "getMax", 0, 0, EScript::create(thisObj->getMax()))
	ES_MFUN(typeObject, BoundingBox2D, "setMax", 1, 1, (thisObj->setMin(parameter[0].to<Vec2>(rt)), thisEObj) )
	ES_MFUN(typeObject, BoundingBox2D, "getCenter", 0, 0, EScript::create(thisObj->getCenter()))
	
	ES_MFUN(typeObject, BoundingBox2D, "contains", 1, 1, thisObj->contains(parameter[0].to<Vec2>(rt)))
	ES_MFUN(typeObject, BoundingBox2D, "intersects", 1, 1, thisObj->intersects(parameter[0].to<BoundingBox2D*>(rt)))
}

}
}
	