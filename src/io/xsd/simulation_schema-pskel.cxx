// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "simulation_schema-pskel.hxx"

// positionType_pskel
//

void positionType_pskel::
x_parser (::xml_schema::decimal_pskel& p)
{
  this->x_parser_ = &p;
}

void positionType_pskel::
y_parser (::xml_schema::decimal_pskel& p)
{
  this->y_parser_ = &p;
}

void positionType_pskel::
z_parser (::xml_schema::decimal_pskel& p)
{
  this->z_parser_ = &p;
}

void positionType_pskel::
parsers (::xml_schema::decimal_pskel& x,
         ::xml_schema::decimal_pskel& y,
         ::xml_schema::decimal_pskel& z)
{
  this->x_parser_ = &x;
  this->y_parser_ = &y;
  this->z_parser_ = &z;
}

positionType_pskel::
positionType_pskel ()
: x_parser_ (0),
  y_parser_ (0),
  z_parser_ (0)
{
}

// gridDimType_pskel
//

void gridDimType_pskel::
dimx_parser (::xml_schema::integer_pskel& p)
{
  this->dimx_parser_ = &p;
}

void gridDimType_pskel::
dimy_parser (::xml_schema::integer_pskel& p)
{
  this->dimy_parser_ = &p;
}

void gridDimType_pskel::
dimz_parser (::xml_schema::integer_pskel& p)
{
  this->dimz_parser_ = &p;
}

void gridDimType_pskel::
parsers (::xml_schema::integer_pskel& dimx,
         ::xml_schema::integer_pskel& dimy,
         ::xml_schema::integer_pskel& dimz)
{
  this->dimx_parser_ = &dimx;
  this->dimy_parser_ = &dimy;
  this->dimz_parser_ = &dimz;
}

gridDimType_pskel::
gridDimType_pskel ()
: dimx_parser_ (0),
  dimy_parser_ (0),
  dimz_parser_ (0)
{
}

// velocityType_pskel
//

void velocityType_pskel::
x_parser (::xml_schema::decimal_pskel& p)
{
  this->x_parser_ = &p;
}

void velocityType_pskel::
y_parser (::xml_schema::decimal_pskel& p)
{
  this->y_parser_ = &p;
}

void velocityType_pskel::
z_parser (::xml_schema::decimal_pskel& p)
{
  this->z_parser_ = &p;
}

void velocityType_pskel::
parsers (::xml_schema::decimal_pskel& x,
         ::xml_schema::decimal_pskel& y,
         ::xml_schema::decimal_pskel& z)
{
  this->x_parser_ = &x;
  this->y_parser_ = &y;
  this->z_parser_ = &z;
}

velocityType_pskel::
velocityType_pskel ()
: x_parser_ (0),
  y_parser_ (0),
  z_parser_ (0)
{
}

// configuration_pskel
//

void configuration_pskel::
fps_parser (::xml_schema::integer_pskel& p)
{
  this->fps_parser_ = &p;
}

void configuration_pskel::
video_length_parser (::xml_schema::integer_pskel& p)
{
  this->video_length_parser_ = &p;
}

void configuration_pskel::
delta_t_parser (::xml_schema::decimal_pskel& p)
{
  this->delta_t_parser_ = &p;
}

void configuration_pskel::
end_time_parser (::xml_schema::decimal_pskel& p)
{
  this->end_time_parser_ = &p;
}

void configuration_pskel::
cuboid_parser (::cuboid_pskel& p)
{
  this->cuboid_parser_ = &p;
}

void configuration_pskel::
parsers (::xml_schema::integer_pskel& fps,
         ::xml_schema::integer_pskel& video_length,
         ::xml_schema::decimal_pskel& delta_t,
         ::xml_schema::decimal_pskel& end_time,
         ::cuboid_pskel& cuboid)
{
  this->fps_parser_ = &fps;
  this->video_length_parser_ = &video_length;
  this->delta_t_parser_ = &delta_t;
  this->end_time_parser_ = &end_time;
  this->cuboid_parser_ = &cuboid;
}

configuration_pskel::
configuration_pskel ()
: fps_parser_ (0),
  video_length_parser_ (0),
  delta_t_parser_ (0),
  end_time_parser_ (0),
  cuboid_parser_ (0)
{
}

// cuboid_pskel
//

void cuboid_pskel::
position_parser (::positionType_pskel& p)
{
  this->position_parser_ = &p;
}

void cuboid_pskel::
grid_dim_parser (::gridDimType_pskel& p)
{
  this->grid_dim_parser_ = &p;
}

void cuboid_pskel::
grid_spacing_parser (::xml_schema::decimal_pskel& p)
{
  this->grid_spacing_parser_ = &p;
}

void cuboid_pskel::
temperature_parser (::xml_schema::decimal_pskel& p)
{
  this->temperature_parser_ = &p;
}

void cuboid_pskel::
mass_parser (::xml_schema::decimal_pskel& p)
{
  this->mass_parser_ = &p;
}

void cuboid_pskel::
velocity_parser (::velocityType_pskel& p)
{
  this->velocity_parser_ = &p;
}

void cuboid_pskel::
type_parser (::xml_schema::integer_pskel& p)
{
  this->type_parser_ = &p;
}

void cuboid_pskel::
parsers (::positionType_pskel& position,
         ::gridDimType_pskel& grid_dim,
         ::xml_schema::decimal_pskel& grid_spacing,
         ::xml_schema::decimal_pskel& temperature,
         ::xml_schema::decimal_pskel& mass,
         ::velocityType_pskel& velocity,
         ::xml_schema::integer_pskel& type)
{
  this->position_parser_ = &position;
  this->grid_dim_parser_ = &grid_dim;
  this->grid_spacing_parser_ = &grid_spacing;
  this->temperature_parser_ = &temperature;
  this->mass_parser_ = &mass;
  this->velocity_parser_ = &velocity;
  this->type_parser_ = &type;
}

cuboid_pskel::
cuboid_pskel ()
: position_parser_ (0),
  grid_dim_parser_ (0),
  grid_spacing_parser_ (0),
  temperature_parser_ (0),
  mass_parser_ (0),
  velocity_parser_ (0),
  type_parser_ (0)
{
}

// positionType_pskel
//

void positionType_pskel::
x (double)
{
}

void positionType_pskel::
y (double)
{
}

void positionType_pskel::
z (double)
{
}

void positionType_pskel::
post_positionType ()
{
}

bool positionType_pskel::
_start_element_impl (const ::xml_schema::ro_string& ns,
                     const ::xml_schema::ro_string& n,
                     const ::xml_schema::ro_string* t)
{
  XSD_UNUSED (t);

  if (this->::xml_schema::complex_content::_start_element_impl (ns, n, t))
    return true;

  if (n == "x" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->x_parser_;

    if (this->x_parser_)
      this->x_parser_->pre ();

    return true;
  }

  if (n == "y" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->y_parser_;

    if (this->y_parser_)
      this->y_parser_->pre ();

    return true;
  }

  if (n == "z" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->z_parser_;

    if (this->z_parser_)
      this->z_parser_->pre ();

    return true;
  }

  return false;
}

bool positionType_pskel::
_end_element_impl (const ::xml_schema::ro_string& ns,
                   const ::xml_schema::ro_string& n)
{
  if (this->::xml_schema::complex_content::_end_element_impl (ns, n))
    return true;

  if (n == "x" && ns.empty ())
  {
    if (this->x_parser_)
      this->x (this->x_parser_->post_decimal ());

    return true;
  }

  if (n == "y" && ns.empty ())
  {
    if (this->y_parser_)
      this->y (this->y_parser_->post_decimal ());

    return true;
  }

  if (n == "z" && ns.empty ())
  {
    if (this->z_parser_)
      this->z (this->z_parser_->post_decimal ());

    return true;
  }

  return false;
}

// gridDimType_pskel
//

void gridDimType_pskel::
dimx (long long)
{
}

void gridDimType_pskel::
dimy (long long)
{
}

void gridDimType_pskel::
dimz (long long)
{
}

void gridDimType_pskel::
post_gridDimType ()
{
}

bool gridDimType_pskel::
_start_element_impl (const ::xml_schema::ro_string& ns,
                     const ::xml_schema::ro_string& n,
                     const ::xml_schema::ro_string* t)
{
  XSD_UNUSED (t);

  if (this->::xml_schema::complex_content::_start_element_impl (ns, n, t))
    return true;

  if (n == "dimx" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->dimx_parser_;

    if (this->dimx_parser_)
      this->dimx_parser_->pre ();

    return true;
  }

  if (n == "dimy" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->dimy_parser_;

    if (this->dimy_parser_)
      this->dimy_parser_->pre ();

    return true;
  }

  if (n == "dimz" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->dimz_parser_;

    if (this->dimz_parser_)
      this->dimz_parser_->pre ();

    return true;
  }

  return false;
}

bool gridDimType_pskel::
_end_element_impl (const ::xml_schema::ro_string& ns,
                   const ::xml_schema::ro_string& n)
{
  if (this->::xml_schema::complex_content::_end_element_impl (ns, n))
    return true;

  if (n == "dimx" && ns.empty ())
  {
    if (this->dimx_parser_)
      this->dimx (this->dimx_parser_->post_integer ());

    return true;
  }

  if (n == "dimy" && ns.empty ())
  {
    if (this->dimy_parser_)
      this->dimy (this->dimy_parser_->post_integer ());

    return true;
  }

  if (n == "dimz" && ns.empty ())
  {
    if (this->dimz_parser_)
      this->dimz (this->dimz_parser_->post_integer ());

    return true;
  }

  return false;
}

// velocityType_pskel
//

void velocityType_pskel::
x (double)
{
}

void velocityType_pskel::
y (double)
{
}

void velocityType_pskel::
z (double)
{
}

void velocityType_pskel::
post_velocityType ()
{
}

bool velocityType_pskel::
_start_element_impl (const ::xml_schema::ro_string& ns,
                     const ::xml_schema::ro_string& n,
                     const ::xml_schema::ro_string* t)
{
  XSD_UNUSED (t);

  if (this->::xml_schema::complex_content::_start_element_impl (ns, n, t))
    return true;

  if (n == "x" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->x_parser_;

    if (this->x_parser_)
      this->x_parser_->pre ();

    return true;
  }

  if (n == "y" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->y_parser_;

    if (this->y_parser_)
      this->y_parser_->pre ();

    return true;
  }

  if (n == "z" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->z_parser_;

    if (this->z_parser_)
      this->z_parser_->pre ();

    return true;
  }

  return false;
}

bool velocityType_pskel::
_end_element_impl (const ::xml_schema::ro_string& ns,
                   const ::xml_schema::ro_string& n)
{
  if (this->::xml_schema::complex_content::_end_element_impl (ns, n))
    return true;

  if (n == "x" && ns.empty ())
  {
    if (this->x_parser_)
      this->x (this->x_parser_->post_decimal ());

    return true;
  }

  if (n == "y" && ns.empty ())
  {
    if (this->y_parser_)
      this->y (this->y_parser_->post_decimal ());

    return true;
  }

  if (n == "z" && ns.empty ())
  {
    if (this->z_parser_)
      this->z (this->z_parser_->post_decimal ());

    return true;
  }

  return false;
}

// configuration_pskel
//

void configuration_pskel::
fps (long long)
{
}

void configuration_pskel::
video_length (long long)
{
}

void configuration_pskel::
delta_t (double)
{
}

void configuration_pskel::
end_time (double)
{
}

void configuration_pskel::
cuboid ()
{
}

void configuration_pskel::
post_configuration ()
{
}

bool configuration_pskel::
_start_element_impl (const ::xml_schema::ro_string& ns,
                     const ::xml_schema::ro_string& n,
                     const ::xml_schema::ro_string* t)
{
  XSD_UNUSED (t);

  if (this->::xml_schema::complex_content::_start_element_impl (ns, n, t))
    return true;

  if (n == "fps" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->fps_parser_;

    if (this->fps_parser_)
      this->fps_parser_->pre ();

    return true;
  }

  if (n == "video_length" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->video_length_parser_;

    if (this->video_length_parser_)
      this->video_length_parser_->pre ();

    return true;
  }

  if (n == "delta_t" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->delta_t_parser_;

    if (this->delta_t_parser_)
      this->delta_t_parser_->pre ();

    return true;
  }

  if (n == "end_time" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->end_time_parser_;

    if (this->end_time_parser_)
      this->end_time_parser_->pre ();

    return true;
  }

  if (n == "cuboid" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->cuboid_parser_;

    if (this->cuboid_parser_)
      this->cuboid_parser_->pre ();

    return true;
  }

  return false;
}

bool configuration_pskel::
_end_element_impl (const ::xml_schema::ro_string& ns,
                   const ::xml_schema::ro_string& n)
{
  if (this->::xml_schema::complex_content::_end_element_impl (ns, n))
    return true;

  if (n == "fps" && ns.empty ())
  {
    if (this->fps_parser_)
      this->fps (this->fps_parser_->post_integer ());

    return true;
  }

  if (n == "video_length" && ns.empty ())
  {
    if (this->video_length_parser_)
      this->video_length (this->video_length_parser_->post_integer ());

    return true;
  }

  if (n == "delta_t" && ns.empty ())
  {
    if (this->delta_t_parser_)
      this->delta_t (this->delta_t_parser_->post_decimal ());

    return true;
  }

  if (n == "end_time" && ns.empty ())
  {
    if (this->end_time_parser_)
      this->end_time (this->end_time_parser_->post_decimal ());

    return true;
  }

  if (n == "cuboid" && ns.empty ())
  {
    if (this->cuboid_parser_)
    {
      this->cuboid_parser_->post_cuboid ();
      this->cuboid ();
    }

    return true;
  }

  return false;
}

// cuboid_pskel
//

void cuboid_pskel::
position ()
{
}

void cuboid_pskel::
grid_dim ()
{
}

void cuboid_pskel::
grid_spacing (double)
{
}

void cuboid_pskel::
temperature (double)
{
}

void cuboid_pskel::
mass (double)
{
}

void cuboid_pskel::
velocity ()
{
}

void cuboid_pskel::
type (long long)
{
}

void cuboid_pskel::
post_cuboid ()
{
}

bool cuboid_pskel::
_start_element_impl (const ::xml_schema::ro_string& ns,
                     const ::xml_schema::ro_string& n,
                     const ::xml_schema::ro_string* t)
{
  XSD_UNUSED (t);

  if (this->::xml_schema::complex_content::_start_element_impl (ns, n, t))
    return true;

  if (n == "position" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->position_parser_;

    if (this->position_parser_)
      this->position_parser_->pre ();

    return true;
  }

  if (n == "grid_dim" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->grid_dim_parser_;

    if (this->grid_dim_parser_)
      this->grid_dim_parser_->pre ();

    return true;
  }

  if (n == "grid_spacing" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->grid_spacing_parser_;

    if (this->grid_spacing_parser_)
      this->grid_spacing_parser_->pre ();

    return true;
  }

  if (n == "temperature" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->temperature_parser_;

    if (this->temperature_parser_)
      this->temperature_parser_->pre ();

    return true;
  }

  if (n == "mass" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->mass_parser_;

    if (this->mass_parser_)
      this->mass_parser_->pre ();

    return true;
  }

  if (n == "velocity" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->velocity_parser_;

    if (this->velocity_parser_)
      this->velocity_parser_->pre ();

    return true;
  }

  if (n == "type" && ns.empty ())
  {
    this->::xml_schema::complex_content::context_.top ().parser_ = this->type_parser_;

    if (this->type_parser_)
      this->type_parser_->pre ();

    return true;
  }

  return false;
}

bool cuboid_pskel::
_end_element_impl (const ::xml_schema::ro_string& ns,
                   const ::xml_schema::ro_string& n)
{
  if (this->::xml_schema::complex_content::_end_element_impl (ns, n))
    return true;

  if (n == "position" && ns.empty ())
  {
    if (this->position_parser_)
    {
      this->position_parser_->post_positionType ();
      this->position ();
    }

    return true;
  }

  if (n == "grid_dim" && ns.empty ())
  {
    if (this->grid_dim_parser_)
    {
      this->grid_dim_parser_->post_gridDimType ();
      this->grid_dim ();
    }

    return true;
  }

  if (n == "grid_spacing" && ns.empty ())
  {
    if (this->grid_spacing_parser_)
      this->grid_spacing (this->grid_spacing_parser_->post_decimal ());

    return true;
  }

  if (n == "temperature" && ns.empty ())
  {
    if (this->temperature_parser_)
      this->temperature (this->temperature_parser_->post_decimal ());

    return true;
  }

  if (n == "mass" && ns.empty ())
  {
    if (this->mass_parser_)
      this->mass (this->mass_parser_->post_decimal ());

    return true;
  }

  if (n == "velocity" && ns.empty ())
  {
    if (this->velocity_parser_)
    {
      this->velocity_parser_->post_velocityType ();
      this->velocity ();
    }

    return true;
  }

  if (n == "type" && ns.empty ())
  {
    if (this->type_parser_)
      this->type (this->type_parser_->post_integer ());

    return true;
  }

  return false;
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

