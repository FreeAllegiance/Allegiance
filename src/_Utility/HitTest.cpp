/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	HitTest.cpp
**
**  Author: 
**
**  Description:
**
**  History:
*/
#include "pch.h"

void    HitTest::SetBB(Time     start,
                       Time     stop,
                       float    dt)
{
    ZAssert (stop >= start);
    ZAssert (m_timeStop >= m_timeStart);

    if (m_staticF)
    {
        m_tStart = 0.0f;
        m_tStop = dt;

        m_lastTest = NULL;
    }
    else
    {
        if (m_timeStart == m_timeStop)
        {
            //hittest always exists (or, at least, started before
            //start and ends after stop)
            m_tStart = 0.0f;
            m_tStop = dt;

            m_stopPosition = GetPosition() + m_velocity * m_tStop;
        }
        else if (stop < m_timeStart)
        {
            //The object shouldn't exist yet ... flag it as dead
            m_deadF = true;
            m_stopPosition = GetPosition();
        }
	    else if (m_timeStop > start)
        {
            if (start < m_timeStart)
            {
                //The object does not come into existance until after start.
                m_tStart = m_timeStart - start;
                //Back the object up so that it is in the correct position at time 0.
                SetPosition(GetPosition() - m_velocity * m_tStart);
                m_deadF = false;     //just in case it was disabled by the above test
            }
            else
            {
                m_tStart = 0.0f;
                ZAssert (!m_deadF);
            }

            m_tStop = ((m_timeStop < stop) ? m_timeStop : stop) - start;

            m_stopPosition = GetPosition() + m_velocity * m_tStop;
        }
        else
        {
            //The will die before the time interval of interest starts
            m_deadF = true;
            m_stopPosition = GetPosition();
        }

        ZAssert (m_stopPosition.LengthSquared() >= 0.0f);
        ZAssert (m_stopPosition.LengthSquared() < 1.0e12f);
        UpdateBB();
    }
}

void    HitTest::UpdateBB(void)
{
    if (!m_staticF)
    {
        Vector  startPosition = m_tStart == 0.0f
                                ? GetPosition()
                                : GetPosition() + m_velocity * m_tStart;   //offset for delayed start

        //let's assume were using the xy & z axes.
        ZAssert (c_nAxes == 3);
        for (int i = 0; (i < c_nAxes); i++)
        {
            float   v1 = startPosition[i];
            float   v2 = m_stopPosition[i];
     
            if (v1 <= v2)
            {
                m_boundingBox.axes[i].values[0] = v1 - m_radius;          
                m_boundingBox.axes[i].values[1] = v2 + m_radius;          
            }
            else
            {
                m_boundingBox.axes[i].values[0] = v2 - m_radius;          
                m_boundingBox.axes[i].values[1] = v1 + m_radius;          
            }

            m_endpoints[i][0].value = m_boundingBox.axes[i].values[0];
            m_endpoints[i][1].value = m_boundingBox.axes[i].values[1];
        }
    }

    m_lastTest = NULL;
}

const double epsilon = 1.0f / 128.0f;

void    HitTest::Collide(HitTest*           pHitTest,
                         CollisionQueue*    pQueue)
{
    ZAssert (pQueue);
    const BoundingBox& hitTestBB = pHitTest->m_boundingBox;

    if ((m_boundingBox.axes[0].values[1] > hitTestBB.axes[0].values[0]) &&
        (m_boundingBox.axes[0].values[0] < hitTestBB.axes[0].values[1]) &&
        (m_boundingBox.axes[1].values[1] > hitTestBB.axes[1].values[0]) &&
        (m_boundingBox.axes[1].values[0] < hitTestBB.axes[1].values[1]) &&
        (m_boundingBox.axes[2].values[1] > hitTestBB.axes[2].values[0]) &&
        (m_boundingBox.axes[2].values[0] < hitTestBB.axes[2].values[1]))
    {
        //Calculate the time when pHitTest and this first collide (if ever)
        Vector  dP = GetPosition() - pHitTest->GetPosition();

		float   r = pHitTest->m_radius + m_radius;
		double  c = (double)(dP * dP) - (double)(r * r);
        Vector  dV = pHitTest->m_velocity - m_velocity;   //negative to eliminate an extraneous - below

        //Distance(t) = |dP - dV * t|
        //Distance(t)^2 = (dP - dV * t) * (dP - dV * t)
        //              = (dP * dP - 2 * dP * dV * t + dV * dV * t^2)
        //Solve for Distance(t)^2 == radius^2 : a*t^2 - b * t + c = 0 => t = (b +- sqrt(b^2 - 4ac)) / 2a
        //
        bool    fCollision = false;
        float   tCollision = 0.0f; // mmf initialize to 0.0f to ensure it is at least defined
        float   tMax = 0.0f; // mmf initialize to 0.0f to ensure it is at least defined

        double  halfB = (dP * dV);  //b/2
        if (halfB > 0.0)   //objects need to be closing at least a little to generate a collision
        {
            double   a = dV * dV;
            if (a > 0.0)   //Shouldn't be a problem but ... float do have their limits of resolution
			{
				double   b2ac = halfB*halfB - a * c;
				if (b2ac >= 0.0)
				{
					//real roots ... at some point (past or future), they'll be close enough to collide
					//pick the minimum time: when they would first collide
					double   s = sqrt(b2ac);
					tCollision = (c <= 0.0) ? 0.0f             //Bounding spheres already intersect
                                            : (float)((halfB - s) / a);
                    tMax = (float)((halfB + s) / a);

                    if (tMax > m_tStop)
                        tMax = m_tStop;

                    fCollision = true;
                }
            }
        }
        else if (c < 0.0)
        {
            //The object bounding sphere's overlap ... we have a possible collision even if the
            //objects are not moving (or are moving apart)
            tCollision = m_tStart;
            tMax = FLT_MAX;
            if (halfB != 0.0)
            {
                //The objects are moving apart ... when will their bounding spheres no longer overlap.
                double   a = dV * dV;
                if (a > 0.0)   //Shouldn't be a problem but ... float do have their limits of resolution
                {
                    tMax = (float)((halfB + (double)sqrt(halfB*halfB - a * c)) / a); // mmf cast sqrt to double instead of float
                }
            }

            if (tMax > m_tStop)
                tMax = m_tStop;
            fCollision = true;
        }

		//Is it in the window of time we are interested in?
		//The range of legal times is always updated in updateBB() and
		//projectiles will always have the most restrictive range (and
		//in a projectile vs. non-projectile, the projectile is always this).
		//
		//Allow collisions that occur slightly beyond m_tStop to guarantee that
		//all collisions get processed at least once (this could lead to collisions
		//happening twice but the less of a problem than missing a collision)
		if (fCollision && (tCollision >= m_tStart) && (tCollision <= m_tStop + (float)epsilon))
        {
            HitTestShape    htsThis = m_shape;
            HitTestShape    htsPHT = pHitTest->m_shape;
            if (((m_shape <= c_htsSphere) && (pHitTest->m_shape <= c_htsSphere)) ||
                HullCollide(&tCollision, tMax + (float)epsilon, this, &htsThis, pHitTest, &htsPHT, dP, dV))
            {
			    pQueue->addCollision(tCollision, this, htsThis, pHitTest, htsPHT);
            }
        }
    }
}

class   BoundingPoint : public HitTest
{
    public:
        BoundingPoint(IObject*    d, bool staticF)
        :
            HitTest(d, 0.0f, staticF, c_htsPoint)
        {
        }
};

class   BoundingSphere : public HitTest
{
    public:
        BoundingSphere(IObject*    d, bool staticF)
        :
            HitTest(d, 0.0f, staticF, c_htsSphere)
        {
        }
};

class   ConvexVertex
{
    public:
        const Vector&       GetPosition(void) const
        {
            return m_position;
        }
        void                SetPosition(const Vector&   p)
        {
            m_position = p;
        }

        const ConvexVertex** GetAdjacencies(void) const
        {
            return  m_adjacencies;
        }

    private:
        Vector                  m_position;
         const ConvexVertex**   m_adjacencies;

    friend class HitTest;
    friend class BoundingHull;
    friend class SingleHull;
};

class   BoundingCone : public HitTest
{
    public:
        BoundingCone(IObject*          d, bool staticF)
        :
            HitTest(d, 0.0f, staticF, c_htsCone)
        {
        }

        Vector  GetExtreme(const Vector&  direction)
        {
            //On the cone portion of the cone
            if (direction.z >= (sqrt2 / 2.0f))
            {
                return Vector(0.0f, 0.0f, GetRadius());   //Near the tip
            }
            else
            {
                //Near the edge ... return the corresponding point on the edge.
                float l = (float)sqrt(direction.x * direction.x + direction.y * direction.y);
                if (l < 0.01f)
                    return Vector(0.0f, 0.0f, 0.0f);
                else
                {
                    float   xy = GetRadius() / l;
                    return Vector(direction.x * xy, direction.y * xy, 0.0f);
                }
            }
        }
};

#undef new

class   SingleHull
{
    public:
        SingleHull(int              nVertices, const Vector&    center)
        :
            m_nVertices(nVertices),
            m_center(center)
        {
        }

        void* operator new(size_t size, int nVertices, int nAdjacencies)
        {
            return (void*)(::new char [size +
                                       sizeof(ConvexVertex) * nVertices +
                                       sizeof(ConvexVertex*) * (nAdjacencies + nVertices)]);
        }

        const ConvexVertex*   GetExtremeVertex(const Vector& direction) const
        {
            return m_pcvExtremes[((direction.x < 0.0) ? 0 : 1) +
                                 ((direction.y < 0.0) ? 0 : 2) +
                                 ((direction.z < 0.0) ? 0 : 4)];
        }

        void                    CalculateExtremeVertices(void)
        {

            static const float octants[][3] = {
                                                {-1.0f, -1.0f, -1.0f},
                                                { 1.0f, -1.0f, -1.0f},
                                                {-1.0f,  1.0f, -1.0f},
                                                { 1.0f,  1.0f, -1.0f},
                                                {-1.0f, -1.0f,  1.0f},
                                                { 1.0f, -1.0f,  1.0f},
                                                {-1.0f,  1.0f,  1.0f},
                                                { 1.0f,  1.0f,  1.0f}
                                              };

            //Pre-find the extreme vertices in each octant
            for (int octant = 0; (octant < 8); octant++)
            {
                const Vector&   direction = *((Vector*)(octants[octant]));

                const ConvexVertex*         pcv = vertex0();
                double                      dotMax = direction * pcv->m_position;
                while (true)
                {
                    const ConvexVertex*     pcvNext = NULL;

                    const ConvexVertex**    ppcvAdjacent = pcv->m_adjacencies;
                    do
                    {
                        double   dot = direction * (*ppcvAdjacent)->m_position;
                        if (dot > dotMax)
                        {
                            dotMax = dot;
                            pcvNext = (*ppcvAdjacent);
                        }
                    }
                    while (*(++ppcvAdjacent) != NULL);

                    if (pcvNext)
                        pcv = pcvNext;
                    else
                    {
                        m_pcvExtremes[octant] = pcv;
                        break;
                    }
                }
            }
        }

        const Vector&   GetCenter(void) const
        {
            return m_center;
        }

    private:
        const ConvexVertex*   vertex0(void) const
        {
            return ((const ConvexVertex*)(((char*)this) + sizeof(*this)));
        }
        const ConvexVertex**  adjacency0(void) const
        {
            return ((const ConvexVertex**)(vertex0() + m_nVertices));
        }

        int                 m_nVertices;
        const ConvexVertex* m_pcvExtremes[8];
        Vector              m_center;

        friend class BoundingHull;
        friend class HitTest;
};

class   MultiHull : public MultiHullBase
{
    public:
        MultiHull(int              nHulls,
                  float            originalRadius,
                  const Vector&    ellipseEquation,
                  float            ellipseRadiusMultiplier)
        :
            MultiHullBase(ellipseEquation, originalRadius),
            m_nHulls(nHulls),
            //m_ellipseEquation(ellipseEquation),
            m_ellipseRadiusMultiplier(ellipseRadiusMultiplier)
        {
        }

        ~MultiHull(void)
        {
            for (int i = 0; (i < m_nHulls); i++)
                delete GetSingleHull(i);
        }

        void* operator new(size_t size, int nHulls)
        {
            return (void*)(::new char [size +
                                       sizeof(SingleHull*) * nHulls]);
        }

        int             GetNhulls(void) const
        {
            return m_nHulls;
        }

        SingleHull*     GetSingleHull(int hullID) const
        {
            ZAssert (hullID >= 0);
            ZAssert (hullID < m_nHulls);
            return hull0()[hullID];
        }

    private:
        SingleHull**    hull0(void) const
        {
            return ((SingleHull**)(((char*)this) + sizeof(*this)));
        }

        int             m_nHulls;
        float           m_ellipseRadiusMultiplier;  //>= m_originalRadius
        //Vector          m_ellipseEquation;          //(x/ee.x)^2 + (y/ee.y)^2 + (z/ee.z)^2 <= 1

        friend class    BoundingHull;
        friend class    HitTest;
};

class   BoundingHull : public HitTest
{
    public:
        BoundingHull(IObject*           d,
                     bool               staticF,
                     const MultiHull*   pMultiHull)
        :
            HitTest(d, pMultiHull->m_originalRadius, staticF, pMultiHull->GetNhulls()),
            m_pMultiHull(pMultiHull)
        {
            const ConvexVertex**  pcvRecent = recentVertex0();
            for (int i = pMultiHull->GetNhulls() - 1; (i >= 0); i--)
                pcvRecent[i] = pMultiHull->GetSingleHull(i)->vertex0();
        }

        void* operator new(size_t size, const MultiHull* pMultiHull)
        {
            return (void*)(::new char [size +
                                       sizeof(ConvexVertex*) * pMultiHull->GetNhulls()]);
        }

        virtual void    UpdateStaticBB(void)
        {
            HitTest::UpdateStaticBB(m_desiredRadius * m_pMultiHull->m_ellipseRadiusMultiplier);
        }

        Vector    GetCenter(HitTestShape  hts) const
        {
            ZAssert (hts >= c_htsConvexHullMin);
            return m_pMultiHull->GetSingleHull(hts)->GetCenter() * m_scale;
        }

        Vector  GetEllipseExtreme(const Vector&  direction)
        {
            //Get the extreme point of an ellipse in direction
            //The ellipse equation is: (x/a)^2 + (y/b)^2 + (z/c)^2 <= 1
            //
            //The normal to the ellipse at (x,y,z) is (x/a^2, y/b^2, z/c^2)
            //and the normal must be parallel to the direction. Therefore:
            //  direction = Dxyz = k * (x/a^2, y/b^2, z/c^2)
            //  x = a^2 Dx/k, y = b^2 Dy/k, z = c^2 Dz / k
            //  and a^2 Dx^2 + b^2 Dy^2 + c^2 Dz^2 = k^2

            double   a2 = m_ellipseEquation.x * m_ellipseEquation.x;
            double   b2 = m_ellipseEquation.y * m_ellipseEquation.y;
            double   c2 = m_ellipseEquation.z * m_ellipseEquation.z;

            double   rk = 1.0 / sqrt(a2 * direction.x * direction.x +
                                     b2 * direction.y * direction.y +
                                     c2 * direction.z * direction.z);

            Vector  extreme((float)(direction.x * a2 * rk),
                            (float)(direction.y * b2 * rk),
                            (float)(direction.z * c2 * rk));


            return extreme;
        }

        #pragma optimize ("p", on)
        Vector  GetHullExtreme(int  hullID, const Vector&  direction)
        {
            ZAssert (hullID >= 0);
            ZAssert (hullID < m_pMultiHull->GetNhulls());

            ZAssert (direction.LengthSquared() >= 0.98f);
            ZAssert (direction.LengthSquared() <= 1.02f);

            ZAssert (m_pMultiHull);

            const ConvexVertex**    ppcvRecent = &(recentVertex0()[hullID]);
            ZAssert (ppcvRecent);
            const ConvexVertex*     pcv = *ppcvRecent;
            ZAssert (pcv);

            float dotMax = direction * pcv->m_position;
            if (dotMax < 0.0)
            {
                pcv = m_pMultiHull->GetSingleHull(hullID)->GetExtremeVertex(direction);
                dotMax = direction * pcv->m_position;
            }

            int n = 0;
            while (true)
            {
                //Hack to verify that FPU round-off error will not cause an infinite loop in retail
                n++;
                ZRetailAssert (n < 300);
                    
                const ConvexVertex*     pcvNext = NULL;

                const ConvexVertex**    ppcvAdjacent = pcv->m_adjacencies;
                do
                {
                    float dot = direction * (*ppcvAdjacent)->m_position;
                    #ifdef _M_IX86 // should only be a problem on x86 CPUs
                    __asm lea eax, [dot];
                    #endif
                    if (dot > dotMax)
                    {
                        dotMax = dot;
                        pcvNext = (*ppcvAdjacent);
                    }
                }
                while (*(++ppcvAdjacent) != NULL);

                if (pcvNext)
                    pcv = pcvNext;
                else
                {
                    *ppcvRecent = pcv;
                    return pcv->m_position * m_scale;
                }
            }
        }
        #pragma optimize ("", on)

        virtual void            SetShape(HitTestShape   shape)
        {
            //Never upgrade to anything more complex than our true shape
            HitTestShape    s = (shape <= m_shapeTrue) ? shape : m_shapeTrue;

            if (s != m_shape)
            {
                m_shape = s;

                if (m_shape == c_htsEllipse)
                    m_radius = m_desiredRadius * m_pMultiHull->m_ellipseRadiusMultiplier;
                else
                    m_radius = m_desiredRadius;
            }
        }

        virtual void    SetRadius(float r)
        {
            m_desiredRadius = r;
            m_scale = r / m_pMultiHull->m_originalRadius;
            m_ellipseEquation = m_scale * m_pMultiHull->GetEllipseEquation();

            //Add a fudge factor to the bounding sphere for an ellipse
            //which is generally larger than the bounding sphere for either
            //a sphere or a convex hull.
            if (m_shape == c_htsEllipse)
                m_radius = r * m_pMultiHull->m_ellipseRadiusMultiplier;
            else
                m_radius = r;
        }
        virtual float    GetScale(void) const
        {
            return m_scale;
        }
        virtual const Vector*   GetEllipseEquation(void) const
        {
            return &m_ellipseEquation;
        }

        const Vector   GetFrameOffset(const char*  pszFrameName) const
        {
            return m_pMultiHull->GetFrameOffset(pszFrameName) * m_scale;
        }

        const Vector&  GetFrameForward(const char* pszFrameName) const
        {
            return m_pMultiHull->GetFrameOffset(pszFrameName);
        }
        const FrameDataUTL* GetFrame(const char* pszFrameName) const
        {
            return m_pMultiHull->GetFrame(pszFrameName);
        }

    private:
        const ConvexVertex**    recentVertex0(void) const
        {
            return ((const ConvexVertex**)(((char*)this) + sizeof(*this)));
        }

        float               m_desiredRadius;
        float               m_scale;
        Vector              m_ellipseEquation;
        const MultiHull*    m_pMultiHull;
};

class   CachedMultiHull
{
    public:
        ~CachedMultiHull(void)
        {
            delete m_pMultiHull;
        }

        char        m_name[c_cbName];
        MultiHull*  m_pMultiHull;

    friend bool operator!=(const CachedMultiHull& r1, const CachedMultiHull& r2)
    {
        return false;
    }
        
};
typedef Slist_utl<CachedMultiHull> CachedList;
typedef Slink_utl<CachedMultiHull> CachedLink;

CachedList  cachedMultiHulls;

MultiHullBase*  HitTest::Load(const char*    pszFileName)
{
    if ((!pszFileName) || (pszFileName[0] == '\0'))
        return NULL;

    MultiHull* pMultiHull = NULL;

    //Does the name exist in the cache?
    //It could exist but be a non-existant hull
    bool    bFound = false;
    {
        for (CachedLink* pcl = cachedMultiHulls.first();
             (pcl != NULL);
             pcl = pcl->next())
        {
            const CachedMultiHull& cmh = pcl->data();
            if (_stricmp(pszFileName, cmh.m_name) == 0)
            {
                pMultiHull = cmh.m_pMultiHull;
                bFound = true;
                break;
            }
        }
    }

    if (!bFound)
    {
        ZAssert (pMultiHull == NULL);

        //First time we've seen this file ... read it in.
        char    artwork[MAX_PATH];

        HRESULT hr = UTL::getFile(pszFileName, ".cvh", artwork,
                                  true, true);

        if (hr == S_OK)    //Don't bother to try and read a file that doesn't contain a valid convex hull
        {
            FILE*   fileIn = fopen(artwork, "r");

            if (fileIn)
            {
                const int   c_maxLine = 512;
                char        line[c_maxLine];

                float   radius;
                Vector  ee;         //ellipse equation
                float   erm;
                if (fgets(line, c_maxLine, fileIn) &&
                    (sscanf(line, "%f %f %f %f %f",
                            &radius, &ee.x, &ee.y, &ee.z, &erm) == 5))
                {
                    int nHulls;
                    if (fgets(line, c_maxLine, fileIn) &&
                        (sscanf(line, "%d",
                                &nHulls) == 1))
                    {
                        ZAssert (nHulls > 0);
                        ZAssert (nHulls < c_htsConvexHullMax);

                        pMultiHull = new(nHulls) MultiHull(nHulls, radius, ee, erm);
                        SingleHull**    ppsh = pMultiHull->hull0();

                        for (int    hullID = nHulls - 1; (hullID >= 0); hullID--)
                        {
                            int     nVertices;
                            int     nAdjacencies;
                            Vector  center;
                            if (fgets(line, c_maxLine, fileIn) &&
                                (sscanf(line, "%d %d %f %f %f",
                                        &nVertices, &nAdjacencies,
                                        &(center.x), &(center.y), &(center.z)) == 5))
                            {
                                ZAssert (nVertices > 0);
                                ZAssert (nAdjacencies > 0);

                                //Same LHS nonsense as for vertices.
                                center.x = -center.x;

                                SingleHull* psh = new(nVertices, nAdjacencies) SingleHull(nVertices, center);
                                *(ppsh++) = psh;

                                //Hack alert: force a cast away from constness so that we can modify the vertex
                                //as we read it in.
                                ConvexVertex*           pcv = (ConvexVertex*)(psh->vertex0());
                                const ConvexVertex**    ppcvAdjacent = psh->adjacency0();

                                //Read in the vertices and their adjacency information.
                                do
                                {
                                    Vector  xyz;
                                    int     n;
                                    if (fgets(line, c_maxLine, fileIn) &&
                                        (sscanf(line, "%f %f %f %d", &xyz.x, &xyz.y, &xyz.z, &n) == 4))
                                    {
                                        ZAssert (n > 0);

                                        //NYI hack, hack, hack ... objects are yaw'd 180 degrees so apply the same
                                        //transformation the vertices in the convex hull
                                        xyz.x = -xyz.x;

                                        //Initialize the vertex
                                        pcv->SetPosition(xyz);
                                        pcv->m_adjacencies = ppcvAdjacent;

                                        //Read in the adjacent vertex information
                                        do
                                        {
                                            int id;
                                            if (fscanf(fileIn, "%d ", &id) == 1)
                                            {
                                                nAdjacencies--;
                                                ZAssert (nAdjacencies >= 0);
                                                *(ppcvAdjacent++) = psh->vertex0() + id;
                                            }
                                            else
                                                ZAssert (false);
                                        }
                                        while (--n);

                                        *(ppcvAdjacent++) = NULL;
                                    }
                                    else
                                        ZAssert (false);

                                    pcv++;
                                }
                                while (--nVertices);
                                ZAssert (nAdjacencies == 0);

                                psh->CalculateExtremeVertices();
                            }
                        }
                    }
                }

                while (fgets(line, c_maxLine, fileIn))
                {
                    FrameLink*  pfl = new FrameLink;
                    FrameDataUTL*  pfd = &(pfl->data());

                    ZAssert (strlen(line) > 0);
                    ZAssert (strlen(line) < c_cbFrameName);
                    line[strlen(line) - 1] = '\0';      //Trim off the \n
                    strcpy(pfd->szName, line);

                    ZVerify(fgets(line, c_maxLine, fileIn));
                    sscanf(line, "%f %f %f",
                            &(pfd->position.x),
                            &(pfd->position.y),
                            &(pfd->position.z));

                    ZVerify(fgets(line, c_maxLine, fileIn));
                    sscanf(line, "%f %f %f",
                            &(pfd->forward.x),
                            &(pfd->forward.y),
                            &(pfd->forward.z));

                    //Hack ... objects are flipped when they are read in so do it for the frames as well.
                    pfd->position.x = -pfd->position.x;

                    //Forward directions are completely reversed (and need X flipped ... so flip y & z)
                    pfd->forward.y = -pfd->forward.y;
                    pfd->forward.z = -pfd->forward.z;

                    pMultiHull->m_frames.last(pfl);
                }

                fclose(fileIn);
            }
        }

        //Create a cache entry for this convex hull file (whether or not we were
        //actually able to load the .cvh file)
        {
            CachedLink*         pcl = new CachedLink;
            CachedMultiHull&    cmh = pcl->data();
            ZAssert (strlen(pszFileName) < c_cbName);
            strcpy(cmh.m_name, pszFileName);
            cmh.m_pMultiHull = pMultiHull;

            //Put it at the front of the list (since we might read another one in real soon).
            cachedMultiHulls.first(pcl);
        }
    }

    return pMultiHull;
}

HitTest*    HitTest::Create(const char*   pszFileName,
                            IObject*      data,
                            bool          staticF,
                            HitTestShape  htsDefault)
{
    HitTest*    pHitTest = NULL;

    if (pszFileName)
    {
        //An unsafe upcast ... but we know better.
        MultiHull* pMultiHull = (MultiHull*)Load(pszFileName);
        if (pMultiHull)
        {
            pHitTest = new (pMultiHull) BoundingHull(data, staticF, pMultiHull);
        }
    }

    if (!pHitTest)
    {
        switch (htsDefault)
        {
            case c_htsSphere:
                pHitTest = (HitTest*)(new BoundingSphere(data, staticF));
            break;
            case c_htsPoint:
                pHitTest = (HitTest*)(new BoundingPoint(data, staticF));
            break;
            case c_htsCone:
                pHitTest = (HitTest*)(new BoundingCone(data, staticF));
            break;
            default:
                ZAssert (false);
        }
    }

    return pHitTest;
}

Vector    HitTest::GetMinExtreme(HitTestShape         hts,
                                 const Vector&        position,
                                 const Orientation&   orientation,
                                 const Vector&        direction)
{
    switch (hts)
    {
        case c_htsPoint:
            return position;

        case c_htsEllipse:
            return position + ((BoundingHull*)this)->GetEllipseExtreme(orientation.TimesInverse(-direction)) * orientation;

        case c_htsCone:
            return position + ((BoundingCone*)this)->GetExtreme(orientation.TimesInverse(-direction)) * orientation;

        case c_htsSphere:
            return position - direction * m_radius;

        default:
            return position + ((BoundingHull*)this)->GetHullExtreme(hts, orientation.TimesInverse(-direction)) * orientation;
    }
}

Vector    HitTest::GetMaxExtreme(HitTestShape       hts,
                                 const Vector&      direction)
{
    switch (hts)
    {
        case c_htsPoint:
            return Vector::GetZero();

        case c_htsEllipse:
            return ((BoundingHull*)this)->GetEllipseExtreme(direction);

        case c_htsCone:
            return ((BoundingCone*)this)->GetExtreme(direction);

        case c_htsSphere:
            return direction * m_radius;

        default:
            return ((BoundingHull*)this)->GetHullExtreme(hts, direction);
    }
}

static bool     DoGilbert(HitTest*              phtObjectA,
                          HitTestShape          htsA,
                          const Vector&         positionStart,
                          const Vector&         positionStop,
                          const Vector&         dV,
                          const Orientation&    orientationHullA,
                          HitTest*              phtObjectB,
                          HitTestShape          htsB,
                          Vector                simplex[4]);

bool HitTest::HullCollide(float*          tStart,
                          float           tMax,
                          HitTest*        phtHullA,
                          HitTestShape*   phtsA,
                          HitTest*        phtHullB,
                          HitTestShape*   phtsB,
                          const Vector&   dP,
                          const Vector&   dV)
{
    ZAssert (tStart);
    ZAssert (phtHullA);
    ZAssert (phtHullB);

    //In general, phtHullB is more complex than phtHullA ... so optimize things to minimize the manipulations of phtHullB
    //In particular, re-orient everything so that we are in B's local coordinate space (dP & dV are aready wrt B).
    const Orientation&  oB = phtHullB->GetOrientation();

    //A common special case is a c_htsPoint vs. c_htsEllipse, so test that explicitly
    if ((phtHullA->m_shape == c_htsPoint) && (phtHullB->m_shape == c_htsEllipse))
    {
        Vector  p = oB.TimesInverse(dP - *tStart * dV);

        //Point vs. ellipse ... distort the coordinate system based on the ellipse equation of phtHullB (which we can do since we
        //are in phtHullB's local coordinate system).
        const Vector&   ee = *(phtHullB->GetEllipseEquation());

        //Now ... when does a point starting at p and moving -v intersect a sphere with radius 1.0 around the origin
        //P(t) = p - v * t; P(t)^2 = 1 = p*p - 2v*p*t + v*v*t^2
        //
        //v*v*t^2 - 2v*p*t + p*p-1 = 0
        //(v*v/2) t^2 - v*p t + ((p*p-1)/2) = 0
        //
        //(a/2) t^2 - b t + (c/2) = 0
        //      t = (b +- sqrt(b*b-4(a/2)(c/2))) / (2 * (a/2)) = (b +- sqrt(b*b-ac)) / a

        p.x /= ee.x;
        p.y /= ee.y;
        p.z /= ee.z;
        double   c = p * p - 1.0;

        if (c <= 0.0)
        {
            //Special case ... point is on or inside the ellipse at the start of the interval
            *phtsA = phtHullA->m_shape;
            *phtsB = phtHullB->m_shape;
            return true;
        }
        //c > 0

        Vector  v = oB.TimesInverse(dV);
        v.x /= ee.x;
        v.y /= ee.y;
        v.z /= ee.z;

        double   b = v * p;
        if (b > 0.0)
        {
            //The point is moving closer to the ellipse ... continue
            double   a = v * v;         //>= 0

            double   bac = b*b - a * c; //a*c >= 0.0 so bac <= b*b
            if (bac >= 0.0)
            {
                //Starting from *tStart so ... calculate time time of collision appropriately
                float   t = *tStart + float((b - sqrt(bac)) / a);
                if (t <= tMax)
                {
                    *tStart = t;
                    *phtsA = phtHullA->m_shape;
                    *phtsB = phtHullB->m_shape;
                    return true;
                }
            }
        }

        return false;
    }
    else
    {
        Vector  p = oB.TimesInverse(dP);
        Vector  v = oB.TimesInverse(dV);

        const Orientation&  oA = phtHullA->GetOrientation();
        Orientation o = oA.TimesInverse(oB);        // == oA * oB^-1

        const double c_maxDelta = 0.25;
        double       speed = dV.Length();

        //Hack alert ... only phtHullB should be subject to the on the fly adjustment
        //of the hit test shape ... so fiddle with their shape.
        HitTestShape    htsA = phtHullA->m_shape;
        HitTestShape    htsB = ((phtHullB->m_pvUseTrueShapeSelf == NULL) ||
                                (phtHullB->m_pvUseTrueShapeSelf != phtHullA->m_pvUseTrueShapeOther)) ? phtHullB->m_shape : phtHullB->m_shapeTrue;


        HitTestShape    htsAcurrent = (htsA < 0) ? htsA : 0;

        float   tOriginalStart = *tStart;
        bool    bCollision = false;
        do
        {
            HitTestShape    htsBcurrent = (htsB < 0) ? htsB : 0;
            do
            {
                if (HitTest::IntervalCollide(tOriginalStart, tMax, (speed == 0.0) ? FLT_MAX : (float)(c_maxDelta / speed),
                                             phtHullA, htsAcurrent,
                                             phtHullB, htsBcurrent,
                                             p, v, o,
                                             tStart))
                {
                    bCollision = true;
                    *phtsA = htsAcurrent;
                    *phtsB = htsBcurrent;
                    tMax = *tStart;  //No point in worrying about collisions that happen after colliding with another part
                }
            }
            while (++htsBcurrent < htsB);
        }
        while (++htsAcurrent < htsA);

        return bCollision;
    }
}

bool    HitTest::IntervalCollide(float               tStart,
                                 float               tStop,
                                 float               maxDeltaT,
                                 HitTest*            phtHullA,
                                 HitTestShape        htsA,
                                 HitTest*            phtHullB,
                                 HitTestShape        htsB,
                                 const Vector&       dP,
                                 const Vector&       dV,
                                 const Orientation&  orientationA,
                                 float*              tCollision)
{
    float   tMiddle = (tStart + tStop) / 2.0f;

    //Do the two convex hulls intersect anywhere along the interval between *tCollision and tMax?
    Vector  direction = dP - tMiddle * dV;
    if (htsA >= c_htsConvexHullMin)
        direction += phtHullA->GetCenter(htsA) * orientationA;
    if (htsB >= c_htsConvexHullMin)
        direction -= phtHullB->GetCenter(htsB);

    if ((direction.x == 0.0f) && (direction.y == 0.0f) && (direction.z == 0.0f))
        return true;

    direction.SetNormalize();

    Vector  positionStart = (dP - tStart * dV);
    Vector  positionStop = (dP - tStop * dV);

    //Try 4 times, saving each result to see if we
    //can find a plane that clearly puts the sphere
    //outside the hull
    const int       c_maxAttempts = 4;
    Vector  extremesHullB[c_maxAttempts];
    Vector  extremesHullA[c_maxAttempts];
    int attempt = 0;
    while (true)
    {
        //For hull A, the extreme is found by takeing the extreme in the given direction and then
        //offsetting it by the position at start or the end of the interval (which ever is appropriate:
        //there are two sets of negation cancelling out here).
        extremesHullA[attempt] = phtHullA->GetMinExtreme(htsA, (direction * dV) >= 0.0f ? positionStop : positionStart,
                                                         orientationA, direction);
        extremesHullB[attempt] = phtHullB->GetMaxExtreme(htsB, direction);

        Vector  delta = extremesHullA[attempt] - extremesHullB[attempt];
        double  distance = (delta * direction);
        if (distance > 0.0)
        {
            //There is no collision anywyare along the interval ... bye
            return false;
        }
        else if (attempt++ >= c_maxAttempts - 1)
            break;

        //Didn't find a good separating plane ... munge direction in the hopes of finding a better one
        //reflect direction around the vector from the extreme to the sphere
        //Get the vector from direction to its projection onto -delta
        direction -= delta * (2.0f * (direction * delta) / delta.LengthSquared());

        ZAssert (direction.Length() >= 0.98f);
        ZAssert (direction.Length() <= 1.02f);
    }

    //We made 4 attempts to find a separating plane and failed.
    //Invoke Gilbert's algorithm.
    Vector  simplex[4];
    {
        int i = 0;
        do
            simplex[i] = extremesHullA[i] - extremesHullB[i];
        while (i++ < 3);
    }

    if (!DoGilbert(phtHullA, htsA, positionStart, positionStop, dV, orientationA,
                   phtHullB, htsB, simplex))
    {
        //No collision ... also bye
        return false;
    }

    //We have a collision somewhere along the interval ... 
    if (tStop - tStart <= maxDeltaT)
    {
        //The interval is small enough that we really do not care any more.
        *tCollision = tMiddle;
        return true;
    }
    else    //split the interval in two and try again
        return IntervalCollide(tStart, tMiddle, maxDeltaT, phtHullA, htsA, phtHullB, htsB, dP, dV, orientationA, tCollision) ||
               IntervalCollide(tMiddle,  tStop, maxDeltaT, phtHullA, htsA, phtHullB, htsB, dP, dV, orientationA, tCollision);
}

static int closest_vertex(int           v0,
                          const Vector  p[],
                          //double      lambda[],
                          int           index[],
                          Vector*       cp)
{
    //lambda[0] = 1;
    index[0] = v0;
    *cp = p[v0];

    return 1;
}

static int closest_edge(double          e0,
                        double          e1,
                        int             ie0,
                        int             ie1,
                        const Vector    p[],
                        int             index[],
                        Vector*         cp)
{
    double  esum_inv = 1.0f/(e0+e1);

    float   lambda[2];
    lambda[0] = (float)(e0*esum_inv);
    lambda[1] = (float)(e1*esum_inv);

    index[0] = ie0;
    index[1] = ie1;

    *cp = lambda[0] * p[ie0] + lambda[1] * p[ie1];

    return 2;
}


static int closest_face(double          f0,
                        double          f1,
                        double          f2,
                        int             if0,
                        int             if1,
                        int             if2,
                        const Vector    p[],
                        //double        lambda[],
                        int             index[],
                        Vector*         cp)
{
    double  fsum_inv = 1.0 / (f0+f1+f2);

    float   lambda[3];
    lambda[0] = (float)(f0*fsum_inv);
    lambda[1] = (float)(f1*fsum_inv);
    lambda[2] = (float)(f2*fsum_inv);

    index[0] = if0;
    index[1] = if1;
    index[2] = if2;

    *cp = lambda[0] * p[if0] +
          lambda[1] * p[if1] +
          lambda[2] * p[if2];

    return 3;
}

/*
    Johnson algorithm for tetrahedron
    Johnson's algorithm for finding the closest point to the origin on a simplex (3d).

    Inputs:
        p      -- array of n 3d points forming the simplex
        n      -- number of points, must be 1, 2, 3, or 4.

    Outputs:
        cp     -- closest point to origin on simplex
        index  -- indices of simplex points forming feature closest to origin {in increasing order]
        lambda -- coefficients of above indexed points forming cp
                  Each coefficient is nonnegative and their sum is 1.

    Returns: number of simplex points forming closest feature:

        1 = vertex
        2 = edge
        3 = face
        4 = tetrahedron (inside simplex interior)

     The return value is always <= n and >= 1.
*/
static int Johnson2(const Vector    p[2],
                    Vector*         cp,
                    int             index[])
{
    Vector  d = p[1] - p[0];

    double l0 = p[0] * d;
    if (l0 >= 0)
    {
        *cp = p[0];
        index[0] = 0;
        return 1;
    }

    double  l1 = p[1] * d;  //Note ... sense is reversed wrt l0
    if (l1 <= 0)
    {
        *cp = p[1];
        index[0] = 1;
        return 1;
    }

    index[0] = 0;
    index[1] = 1;

    *cp = p[0] + (float)(l0 / (l0 - l1)) * d;

    return 2;
}
/*
#define DOT(a,b) ( (a)[0]*(b)[0] + (a)[1]*(b)[1] + (a)[2]*(b)[2] )

static
int jsny3(double *p,double cp[3], int index[])
{
    double d[3][3];        // array of dot products d[i][j] = <p[i],p[j]> 
    double d2[3][2];       // array of D_k{i,j} encoded as [l][0 if k=i, 1 if k=j] i < j, k in {i,j} 
                           // where l=0 -> {0,1}, l=1 -> {1,2}, l=2 -> {0,2} 
    double d3[3];          // array of D_l{i,j,k} encoded as [l], i < j < k, l in {i,j,k} 
    register double *pi,*pj;
    register int i,j;
    //double min;
    //int n_min;

    // compute dot products 
    pi = p;
    for (i = 0; i < 3; i++) {
        pj = pi;
        for (j = i; j < 3; j++) {
            d[i][j] = d[j][i] = DOT(pi,pj);
            pj += 3;
        }
        pi += 3;
    }

    // compute D2 
    d2[0][0] = d[1][1] - d[1][0];
    d2[0][1] = d[0][0] - d[0][1];
    d2[1][0] = d[2][2] - d[2][1];
    d2[1][1] = d[1][1] - d[1][2];
    d2[2][0] = d[2][2] - d[2][0];
    d2[2][1] = d[0][0] - d[0][2];

    // compute D3 
    d3[0] = d2[1][0]*(d[1][1]-d[1][0]) + d2[1][1]*(d[2][1]-d[2][0]);
    d3[1] = d2[2][0]*(d[0][0]-d[0][1]) + d2[2][1]*(d[2][0]-d[2][1]);
    d3[2] = d2[0][0]*(d[0][0]-d[0][2]) + d2[0][1]*(d[1][0]-d[1][2]);

    debugf("jsny3\n");
    {
        debugf("p[]\n");
        for (int i = 0; (i < 3); i++)
            debugf("%f %f %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);

        debugf("d[][]\n");
        for (int j = 0; (j < 3); j++)
            debugf("%f %f %f\n", d[j][0], d[j][1], d[j][2]);

        debugf("d2[][]\n");
        for (int k = 0; (k < 3); k++)
            debugf("%f %f\n", d2[k][0], d2[k][1]);

        debugf("d3\n");
        debugf("%f %f %f\n", d3[0], d3[1], d3[2]);
    }

    return 0;
}
*/

static int  Johnson3(const Vector   p[3],
                     Vector*        cp,
                     int            index[])
{
    //Mappings between all possible subsets of the vertices and a
    //unique subset ID.
    //  subset1[i]          ({Pi})
    //  subset2[i][j]       ({Pi, Pj})  i != j
    //  subset3             ({P0, P1, P2})
    static const int   subset1[3] =        { 0,  1,  2};
    static const int   subset2[3][3] =    {{-1,  3,  4},
                                           { 3, -1,  5},
                                           { 4,  5, -1}};
    static const int   subset3 = 6;

    double dp[3][3];
    {
        for (int i = 0; (i < 3); i++)
            for (int j = i; (j < 3); j++)
                dp[i][j] = dp[j][i] = (double)(p[i].x) * (double)(p[j].x) +
                                      (double)(p[i].y) * (double)(p[j].y) +
                                      (double)(p[i].z) * (double)(p[j].z);
    }

    double delta[3][subset3 + 1];

    //Generic form: delta[i][{Pi}] = 1.0
    //  but don't bother


    {
        //Generic form: delta[i][{Pj} | {Pi}] = Pj * (Pj - Pi)              [i != j]
        //                                    = dp[j][j] - dp[j][i]

        #define SetDelta(i,j)   delta[i][subset2[j][i]] = dp[j][j] - dp[j][i]

        SetDelta(1, 0);
        SetDelta(2, 0);

        SetDelta(0, 1);
        SetDelta(2, 1);

        SetDelta(0, 2);
        SetDelta(1, 2);

        #undef SetDelta
    }

    {
        //Generic form: delta[i][{Pj, Pk} | {Pi}] = delta[j][{Pj, Pk}] * (Pj * (Pj - Pi)) +         [i != j, k; j < k]
        //                                          delta[k][{Pj, Pk}] * (Pk * (Pj - Pi))
        //                                        = delta[j][{Pj, Pk}] * (dp[j][j] - dp[j][i]) + 
        //                                          delta[k][{Pj, Pk}] * (dp[k][j] - dp[k][i])

        // i != j, k; j < k; x != i, j, k
        #define SetDelta(i, j, k)    delta[i][subset3] = (delta[j][subset2[j][k]] * (dp[j][j] - dp[j][i])) + \
                                                         (delta[k][subset2[j][k]] * (dp[k][j] - dp[k][i]))

        SetDelta(2, 0, 1);
        SetDelta(1, 0, 2);
        SetDelta(0, 1, 2);

        #undef SetDelta
    }

    //Now that we've done all this ... see if we have a solution
    //We have a valid solution if:
    //      delta[i][S]         > 0 (for all i in S)
    //      delta[j][S | {Pj}] <= 0 (for all j not in S)
    /*
    {
        debugf("p[]\n");
        for (int i = 0; (i < 3); i++)
            debugf("%f %f %f\n", p[i].x, p[i].y, p[i].z);

        debugf("dp[][]\n");
        for (int j = 0; (j < 3); j++)
            debugf("%f %f %f\n", dp[j][0], dp[j][1], dp[j][2]);

        debugf("delta[][]\n");
        debugf("%f %f\n", delta[0][subset2[0][1]], delta[1][subset2[0][1]]);
        debugf("%f %f\n", delta[1][subset2[1][2]], delta[2][subset2[1][2]]);
        debugf("%f %f\n", delta[2][subset2[0][2]], delta[2][subset2[0][2]]);

        debugf("d3\n");
        debugf("%f %f %f\n", delta[0][subset3], delta[1][subset3], delta[2][subset3]);
    }
    */

    //Inside for the triangle. The last clause of the test is degenerate
    //since there is no Pj not in S
    if ((delta[0][subset3] > 0.0) &&
        (delta[1][subset3] > 0.0) &&
        (delta[2][subset3] > 0.0))
    {
        //Yes
        return closest_face(delta[0][subset3], delta[1][subset3], delta[2][subset3],
                            0, 1, 2, p, index, cp);
    }

    {
        //Test for each vertex. The first clause of the test is degerate
        //since delta[i][S[i]] = 1
        
        //Test for vertex i
        #define TestDelta(i, j, k)   if ((delta[i][subset2[j][i]] <= 0.0) && \
                                         (delta[i][subset2[k][i]] <= 0.0)) \
                                            return closest_vertex(i, p, index, cp);

        TestDelta(0, 1, 2);
        TestDelta(1, 0, 2);
        TestDelta(2, 0, 1);

        #undef TestDelta
    }

    {
        //Test for each edge i, j
        //  delta[i][subset2[i][j]] > 0
        //  delta[j][subset2[i][j]] > 0
        //
        //  delta[k][subset3[l]] < 0
        //  delta[l][subset3[k]] < 0
        #define TestDelta(i, j, k)   if ((delta[i][subset2[i][j]] > 0.0) && \
                                         (delta[j][subset2[i][j]] > 0.0) && \
                                         (delta[k][subset3] <= 0.0)) \
                                         return closest_edge(delta[i][subset2[i][j]],    \
                                                             delta[j][subset2[i][j]],    \
                                                             i, j, p, index, cp)

        TestDelta(0, 1, 2);
        TestDelta(0, 2, 1);
        TestDelta(1, 2, 0);

        #undef TestDelta
    }

    {
        //Test for each face i, j, k
        //  delta[i][subset3[l]] > 0
        //  delta[j][subset3[l]] > 0
        //  delta[k][subset3[l]] > 0
        //
        //  delta[l][subset4] <= 0.0f

        #define TestDelta(i, j, k)   if ((delta[i][subset3] > 0.0) && \
                                         (delta[j][subset3] > 0.0) && \
                                         (delta[k][subset3] > 0.0))     \
                                         return closest_face(delta[i][subset3],   \
                                                             delta[j][subset3],   \
                                                             delta[k][subset3],   \
                                                             i, j, k, p, index, cp)

        TestDelta(0, 1, 2);

        #undef TestDelta
    }

    //If we haven't found a solution so far (round off or some such) ...
    int     nMin;
    double  dMin2 = HUGE_VAL;

    {
        //Try the vertices
        int v = 0;  // mmf initialize to 0 to ensure it is at least defined
        for (int i = 0; (i < 3); i++)
        {
            if (dp[i][i] < dMin2)
            {
                dMin2 = dp[i][i];
                v = i;
            }
        }

        nMin = closest_vertex(v, p, index, cp);
    }

    {
        //Try the edges
        for (int i = 0; (i < 2); i++)
        {
            for (int j = i + 1; (j < 3); j++)
            {
                if ((delta[i][subset2[i][j]] > 0.0) &&
                    (delta[j][subset2[i][j]] > 0.0))
                {
                    double  d = (delta[i][subset2[i][j]] * dp[i][i] +
                                 delta[j][subset2[i][j]] * dp[j][i]) /
                                (delta[i][subset2[i][j]] + delta[j][subset2[i][j]]);

                    if (d < dMin2)
                    {
                        dMin2 = d;
                        nMin = closest_edge(delta[i][subset2[i][j]],
                                            delta[j][subset2[i][j]],
                                            i, j, p, index, cp);
                    }
                }
            }
        }
    }

    {
        if ((delta[0][subset3] > 0.0) &&
            (delta[1][subset3] > 0.0) &&
            (delta[2][subset3] > 0.0))
        {
            double  d = (delta[0][subset3] * dp[0][0] +
                         delta[1][subset3] * dp[1][0] +
                         delta[2][subset3] * dp[2][0]) /
                        (delta[0][subset3] +
                         delta[1][subset3] +
                         delta[2][subset3]);

            if (d < dMin2)
            {
                dMin2 = d;
                nMin = closest_face(delta[0][subset3],
                                    delta[1][subset3],
                                    delta[2][subset3],
                                    0, 1, 2, p, index, cp);
            }
        }
    }

    return nMin;
}

static int  Johnson4(const Vector   p[4],
                     Vector*        cp,
                     int            index[])
{
    //Mappings between all possible subsets of the vertices and a
    //unique subset ID.
    //  subset1[i]          ({Pi})
    //  subset2[i][j]       ({Pi, Pj}),     i != j
    //  subset3[i]          ({Pj, Pk, Pl}), i != j; i != k; i != l
    //  subset4             ({P0, P1, P2, P3})
    static const int   subset1[4] =        { 0,  1,  2,  3};

    static const int   subset2[4][4] =    {{-1,  4,  5,  6},
                                    { 4, -1,  7,  8},
                                    { 5,  7, -1,  9},
                                    { 6,  8,  9, -1}};
    static const int   subset3[4] =        {10, 11, 12, 13};
    static const int   subset4 = 14;

    double dp[4][4];
    {
        for (int i = 0; (i < 4); i++)
            for (int j = i; (j < 4); j++)
                dp[i][j] = dp[j][i] = (double)(p[i].x) * (double)(p[j].x) +
                                      (double)(p[i].y) * (double)(p[j].y) +
                                      (double)(p[i].z) * (double)(p[j].z);
    }

    double delta[4][subset4 + 1];

    //Generic form: delta[i][{Pi}] = 1.0
    //  but don't bother


    {
        //Generic form: delta[i][{Pj} | {Pi}] = Pj * (Pj - Pi)              [i != j]
        //                                    = dp[j][j] - dp[j][i]

        #define SetDelta(i,j)   delta[i][subset2[j][i]] = dp[j][j] - dp[j][i]

        SetDelta(1, 0);
        SetDelta(2, 0);
        SetDelta(3, 0);

        SetDelta(0, 1);
        SetDelta(2, 1);
        SetDelta(3, 1);

        SetDelta(0, 2);
        SetDelta(1, 2);
        SetDelta(3, 2);

        SetDelta(0, 3);
        SetDelta(1, 3);
        SetDelta(2, 3);

        #undef SetDelta
    }

    {
        //Generic form: delta[i][{Pj, Pk} | {Pi}] = delta[j][{Pj, Pk}] * (Pj * (Pj - Pi)) +         [i != j, k; j < k]
        //                                          delta[k][{Pj, Pk}] * (Pk * (Pj - Pi))
        //
        //                                        = delta[j][{Pj, Pk}] * (dp[j][j] - dp[j][i]) + 
        //                                          delta[k][{Pj, Pk}] * (dp[k][j] - dp[k][i])

        // i != j, k; j < k; x != i, j, k
        #define SetDelta(i, j, k, x)    delta[i][subset3[x]] = (delta[j][subset2[j][k]] * (dp[j][j] - dp[j][i])) + \
                                                               (delta[k][subset2[j][k]] * (dp[k][j] - dp[k][i]))

        SetDelta(2, 0, 1, 3);
        SetDelta(3, 0, 1, 2);

        SetDelta(1, 0, 2, 3);
        SetDelta(3, 0, 2, 1);

        SetDelta(1, 0, 3, 2);
        SetDelta(2, 0, 3, 1);

        SetDelta(0, 1, 2, 3);
        SetDelta(3, 1, 2, 0);

        SetDelta(0, 1, 3, 2);
        SetDelta(2, 1, 3, 0);

        SetDelta(0, 2, 3, 1);
        SetDelta(1, 2, 3, 0);

        #undef SetDelta
    }

    {
        //Generic form: delta[i][{Pj, Pk, Pl} | {Pi}] = delta[j][{Pj, Pk, Pl}] * (Pj * (Pj - Pi)) +     [i != j, k, l; j < k, l, k != l]
        //                                              delta[k][{Pj, Pk, Pl}] * (Pk * (Pj - Pi)) +
        //                                              delta[l][{Pj, Pk, Pl}] * (Pl * (Pj - Pi))
        //                                            = delta[j][{Pj, Pk, Pl}] * (dp[j][j] - dp[j][i]) +
        //                                            = delta[j][{Pj, Pk, Pl}] * (dp[k][j] - dp[k][i]) +
        //                                            = delta[j][{Pj, Pk, Pl}] * (dp[k][j] - dp[l][i])

        //  i != j, k, l; j < k, l, k != l
        #define SetDelta(i, j, k, l)        delta[i][subset4] = delta[j][subset3[i]] * (dp[j][j] - dp[j][i]) + \
                                                                delta[k][subset3[i]] * (dp[k][j] - dp[k][i]) + \
                                                                delta[l][subset3[i]] * (dp[l][j] - dp[l][i])

        SetDelta(0, 1, 2, 3);
        SetDelta(1, 0, 2, 3);
        SetDelta(2, 0, 1, 3);
        SetDelta(3, 0, 1, 2);

        #undef SetDelta
    }

    //Now that we've done all this ... see if we have a solution
    //We have a valid solution if:
    //      delta[i][S]         > 0 (for all i in S)
    //      delta[j][S | {Pj}] <= 0 (for all j not in S)


    //Inside for the tetrahedron. The last clause of the test is degenerate
    //since there is no Pj not in S
    if ((delta[0][subset4] > 0.0) &&
        (delta[1][subset4] > 0.0) &&
        (delta[2][subset4] > 0.0) &&
        (delta[3][subset4] > 0.0))
    {
        //Yes
        *cp = Vector::GetZero();
        return 4;
    }

    {
        //Test for each vertex. The first clause of the test is degerate
        //since delta[i][S[i]] = 1
        
        //Test for vertex i
        #define TestDelta(i, j, k, l)   if ((delta[i][subset2[j][i]] <= 0.0) && \
                                            (delta[i][subset2[k][i]] <= 0.0) && \
                                            (delta[i][subset2[l][i]] <= 0.0)) \
                                            return closest_vertex(i, p, index, cp);

        TestDelta(0, 1, 2, 3);
        TestDelta(1, 0, 2, 3);
        TestDelta(2, 0, 1, 3);
        TestDelta(3, 0, 1, 2);

        #undef TestDelta
    }

    {
        //Test for each edge i, j
        //  delta[i][subset2[i][j]] > 0
        //  delta[j][subset2[i][j]] > 0
        //
        //  delta[k][subset3[l]] < 0
        //  delta[l][subset3[k]] < 0
        #define TestDelta(i, j, k, l)   if ((delta[i][subset2[i][j]] > 0.0) && \
                                            (delta[j][subset2[i][j]] > 0.0) && \
                                            (delta[k][subset3[l]] <= 0.0) && \
                                            (delta[l][subset3[k]] <= 0.0)) \
                                            return closest_edge(delta[i][subset2[i][j]],    \
                                                                delta[j][subset2[i][j]],    \
                                                                i, j, p, index, cp)

        TestDelta(0, 1, 2, 3);
        TestDelta(0, 2, 1, 3);
        TestDelta(0, 3, 1, 2);

        TestDelta(1, 2, 0, 3);
        TestDelta(1, 3, 0, 2);

        TestDelta(2, 3, 0, 1);

        #undef TestDelta
    }

    {
        //Test for each face i, j, k
        //  delta[i][subset3[l]] > 0
        //  delta[j][subset3[l]] > 0
        //  delta[k][subset3[l]] > 0
        //
        //  delta[l][subset4] <= 0.0f

        #define TestDelta(i, j, k, l)   if ((delta[i][subset3[l]] > 0.0) && \
                                            (delta[j][subset3[l]] > 0.0) && \
                                            (delta[k][subset3[l]] > 0.0) && \
                                            (delta[l][subset4] <= 0.0))     \
                                            return closest_face(delta[i][subset3[l]],   \
                                                                delta[j][subset3[l]],   \
                                                                delta[k][subset3[l]],   \
                                                                i, j, k, p, index, cp)

        TestDelta(0, 1, 2, 3);
        TestDelta(0, 1, 3, 2);
        TestDelta(0, 2, 3, 1);
        TestDelta(1, 2, 3, 0);

        #undef TestDelta
    }

    //If we haven't found a solution so far (round off or some such) ...
    int     nMin;
    double  dMin2 = HUGE_VAL;

    {
        //Try the vertices
        int v = 0; // mmf initialize to 0 to ensure it is at least defined
        for (int i = 0; (i < 4); i++)
        {
            if (dp[i][i] < dMin2)
            {
                dMin2 = dp[i][i];
                v = i;
            }
        }

        nMin = closest_vertex(v, p, index, cp);
    }

    {
        //Try the edges
        for (int i = 0; (i < 3); i++)
        {
            for (int j = i + 1; (j < 4); j++)
            {
                if ((delta[i][subset2[i][j]] > 0.0) &&
                    (delta[j][subset2[i][j]] > 0.0))
                {
                    double  d = (delta[i][subset2[i][j]] * dp[i][i] +
                                 delta[j][subset2[i][j]] * dp[j][i]) /
                                (delta[i][subset2[i][j]] + delta[j][subset2[i][j]]);

                    if (d < dMin2)
                    {
                        dMin2 = d;
                        nMin = closest_edge(delta[i][subset2[i][j]],
                                            delta[j][subset2[i][j]],
                                            i, j, p, index, cp);
                    }
                }
            }
        }
    }

    {
        {
            //Try the faces
            const int ids[4][4] = {{0, 1, 2, 3}, {0, 1, 3, 2},
                                   {0, 2, 3, 1}, {1, 2, 3, 0}};
            for (int f = 0; (f < 4); f++)
            {
                int i = ids[f][0];
                int j = ids[f][1];
                int k = ids[f][2];
                int l = ids[f][3];

                if ((delta[i][subset3[l]] > 0.0) &&
                    (delta[j][subset3[l]] > 0.0) &&
                    (delta[k][subset3[l]] > 0.0))
                {
                    double  d = (delta[i][subset3[l]] * dp[i][i] +
                                 delta[j][subset3[l]] * dp[j][i] +
                                 delta[k][subset3[l]] * dp[k][i]) /
                                (delta[i][subset3[l]] +
                                 delta[j][subset3[l]] +
                                 delta[k][subset3[l]]);

                    if (d < dMin2)
                    {
                        dMin2 = d;
                        nMin = closest_face(delta[i][subset3[l]],
                                            delta[j][subset3[l]],
                                            delta[k][subset3[l]],
                                            i, j, k, p, index, cp);
                    }
                }
            }
        }
    }

    return nMin;
}

int Johnson(int             n,
            const Vector    p[],
            Vector*         cp,
            int             index[])
{
    switch (n)
    {
        case 1:
        {
            *cp = p[0];
            index[0] = 0;
            return 1;
        }

        case 2:
        {
            return Johnson2(p, cp, index);
        }
        
        case 3:
        {
            return Johnson3(p, cp, index);
        }

        default:
        {
            return Johnson4(p, cp, index);
        }
    }
}

static bool    DoGilbert(HitTest*              phtHullA,
                         HitTestShape          htsA,
                         const Vector&         positionStart,
                         const Vector&         positionStop,
                         const Vector&         dV,
                         const Orientation&    orientationHullA,
                         HitTest*              phtHullB,
                         HitTestShape          htsB,
                         Vector                simplex[4])
{
    int     nVertices = 3;  //Always start with four (but the three is incremented before it is used).

    static const int    c_maxHistory = 100;
    static  Vector      vertexHistory[c_maxHistory];

    vertexHistory[0] = simplex[0];
    vertexHistory[1] = simplex[1];
    vertexHistory[2] = simplex[2];
    vertexHistory[3] = simplex[3];
    int vertexID = 3;

    Vector  cp;
    Vector  delta;

    while (true)
    {
        int indices[3];
        int nMin = Johnson(++nVertices, simplex, &cp, indices);

        if (nMin == 4)
        {
            return true;
        }

        //Find a new extreme point
        {
            double   l2 = cp.LengthSquared();
            if (l2 < epsilon * epsilon)
                return true;

            cp /= (float)sqrt(l2);
        }

        Vector v0 = phtHullA->GetMinExtreme(htsA, (cp * dV >= 0.0f) ? positionStop : positionStart, orientationHullA, cp) -
                    phtHullB->GetMaxExtreme(htsB, cp);

        if ((v0 * cp) > 0.0)
        {
            //Found a separating plane
            return false;
        }

        //Have we been here before?
        {
            for (int i = vertexID; (i >= 0); i--)
            {
                if ((vertexHistory[i] - v0).LengthSquared() < epsilon * epsilon)
                {
                    //A duplicate point ... so the simplex will not contain the origin.
                    return false;
                }
            }

            ZAssert (vertexID < c_maxHistory - 1);
            vertexHistory[++vertexID] = v0;
        }

        //Otherwise ... eliminate all the points from the simplex
        //that are not in the index array (assume the index array
        //is sorted).
        {
            int i = 0;
            do
            {
                if (indices[i] != i)
                {
                    ZAssert (indices[i] > i);
                    simplex[i] = simplex[indices[i]];
                }
            }
            while (++i < nMin);

            nVertices = i;
        }

        //Replace the last vertex in the simplex array with new vertex
        simplex[nVertices] = v0;
    }

    return false;
}
