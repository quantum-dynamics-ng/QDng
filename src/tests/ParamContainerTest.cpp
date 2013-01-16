/*
 * ParamContainerTest.cpp
 *
 *  Created on: Jan 16, 2013
 *      Author: markus
 */

#include "ParamContainerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ParamContainerTest);

ParamContainerTest::ParamContainerTest()
{
}

ParamContainerTest::~ParamContainerTest()
{
}

void ParamContainerTest::setUp()
{

   p.clear();

   p.SetValue("undef", "ustring");

   int  i=1;
   p.SetValue("int", i);

   size_t s = 3;
   p.SetValue("sizet", s);

   double d = 4.4;
   p.SetValue("double", d);

   bool b = true;
   p.SetValue("bool", b);

   string c("string");
   p.SetValue("string", "string", ParamContainer::t_string);
}

void ParamContainerTest::Type_Test()
{
   CPPUNIT_ASSERT(p.isType("undef", ParamContainer::t_undef));
   CPPUNIT_ASSERT(p.isType("int", ParamContainer::t_int));
   CPPUNIT_ASSERT(p.isType("sizet", ParamContainer::t_size_t));
   CPPUNIT_ASSERT(p.isType("double", ParamContainer::t_double));
   CPPUNIT_ASSERT(p.isType("bool", ParamContainer::t_bool));
   CPPUNIT_ASSERT(p.isType("string", ParamContainer::t_string));

}

void ParamContainerTest::IO_Test()
{
   /* check if reading with types work */
   p.WriteToFile("params_wt", true);
   ParamContainer pwt;
   pwt.ReadFromFile("params_wt");
   CPPUNIT_ASSERT(pwt.isType("undef", ParamContainer::t_undef));
   CPPUNIT_ASSERT(pwt.isType("int", ParamContainer::t_int));
   CPPUNIT_ASSERT(pwt.isType("sizet", ParamContainer::t_size_t));
   CPPUNIT_ASSERT(pwt.isType("double", ParamContainer::t_double));
   CPPUNIT_ASSERT(pwt.isType("bool", ParamContainer::t_bool));
   CPPUNIT_ASSERT(pwt.isType("string", ParamContainer::t_string));

   /* check if reading with undefined types work */
   p.WriteToFile("params");
   ParamContainer pwot;
   pwot.ReadFromFile("params");
   CPPUNIT_ASSERT(pwot.isType("undef", ParamContainer::t_undef));
   CPPUNIT_ASSERT(pwot.isType("int", ParamContainer::t_undef));
   CPPUNIT_ASSERT(pwot.isType("sizet", ParamContainer::t_undef));
   CPPUNIT_ASSERT(pwot.isType("double", ParamContainer::t_undef));
   CPPUNIT_ASSERT(pwot.isType("bool", ParamContainer::t_undef));
   CPPUNIT_ASSERT(pwot.isType("string", ParamContainer::t_undef));

}
