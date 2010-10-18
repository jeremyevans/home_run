require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime.strptime" do
  before do
    @t = Time.now
  end
  
  it "._strptime should be a hash of values" do
    DateTime._strptime('2008-10-11T01:00:02+0000').should == {:offset=>0, :zone=>"+0000", :sec=>2, :year=>2008, :hour=>1, :mday=>11, :min=>0, :mon=>10}
    DateTime._strptime('2008-10-11', '%Y-%m-%d').should == {:year=>2008, :mon=>10, :mday=>11}
  end
  
  it "should have defaults and an optional sg value" do
    DateTime.strptime('2008-10-11T00:00:00+00:00').should == DateTime.civil(2008, 10, 11)
    DateTime.strptime('2008-10-11', '%Y-%m-%d').should == DateTime.civil(2008, 10, 11)
    DateTime.strptime('2008-10-11', '%Y-%m-%d', 1).should == DateTime.civil(2008, 10, 11)
    DateTime.strptime.should == DateTime.jd
  end

  it "should be able to parse the date time" do
    DateTime.strptime("2000-04-06T10:11:12+00:00").should == DateTime.civil(2000, 4, 6, 10, 11, 12)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.strptime.should be_kind_of(c)
    c.strptime('2008-10-11', '%Y-%m-%d').should be_kind_of(c)
  end

  it "should be able to parse the hour in a 24 hour clock with leading zero" do
    DateTime.strptime("10", '%H').should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 0, 0)
    DateTime.strptime("09", '%H').should == DateTime.civil(@t.year, @t.mon, @t.day, 9, 0, 0)
    DateTime.strptime("2000 16", '%Y %H').should == DateTime.civil(2000, 1, 1, 16, 0, 0)
  end

  it "should be able to parse the hour in a 12 hour clock with leading zero with meridian indicator" do
    DateTime.strptime("10 AM", '%I %p').should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 0, 0)
    DateTime.strptime("03 04 04 PM", '%m %d %I %p').should == DateTime.civil(@t.year, 3, 4, 16, 0, 0)
  end
  
  it "should be able to parse the hour in a 24 hour clock with leading space" do
    DateTime.strptime("10", '%k').should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 0, 0)
    DateTime.strptime(" 9", '%k').should == DateTime.civil(@t.year, @t.mon, @t.day, 9, 0, 0)
    DateTime.strptime("10 16", '%d %k').should == DateTime.civil(@t.year, @t.mon, 10, 16, 0, 0)
  end

  it "should be able to parse the hour in a 12 hour clock with leading space with meridian indicator" do
    DateTime.strptime("10 am", '%l %P').should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 0, 0)
    DateTime.strptime(" 4 pm", '%l %P').should == DateTime.civil(@t.year, @t.mon, @t.day, 16, 0, 0)
  end
  
  it "should be able to parse the number of milliseconds of the second" do
    DateTime.strptime("10 000", '%M %L').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0)
    DateTime.strptime("10 500", '%M %L').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0) + (0.5/86400)
  end
  
  it "should be able to parse the number of milliseconds of the second with trailing numbers" do
    DateTime.strptime("10 0000", '%M %L0').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0)
    DateTime.strptime("10 50012", '%M %L%m').should == DateTime.civil(@t.year, 12, 1, 0, 10, 0) + (0.5/86400)
  end
  
  it "should be able to parse the minute with leading zero" do
    DateTime.strptime("10", '%M').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0)
    DateTime.strptime("09", '%M').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 9, 0)
  end
 
  it "should be able to parse the number of nanoseconds of the second" do
    DateTime.strptime("10 000000000", '%M %N').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0)
    DateTime.strptime("10 500000000", '%M %N').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0) + (0.5/86400)
  end
  
  it "should be able to parse the number of nanoseconds of the second with trailing numbers" do
    DateTime.strptime("10 0000000000", '%M %N0').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 10, 0)
    DateTime.strptime("10 50000000012", '%M %N%m').should == DateTime.civil(@t.year, 12, 1, 0, 10, 0) + (0.5/86400)
  end
  
  it "should be able to parse the number of seconds since the unix epoch" do
    Date.strptime("954979200", "%s").should == Date.civil(2000, 4, 6)
  end
  
  it "should be able to parse the number of milliseconds since the unix epoch" do
    DateTime.strptime("1226527410000", '%Q').should == DateTime.civil(2008, 11, 12, 22, 3, 30, 0)
    DateTime.strptime("1226527411000", '%Q').should == DateTime.civil(2008, 11, 12, 22, 3, 31, 0)
    DateTime.strptime("32511888001500", "%Q").should == DateTime.civil(3000, 4, 6, 0, 0, 1) + (0.5/86400)
  end
  
  it "should be able to parse the second with leading zero" do
    DateTime.strptime("10", '%S').should == DateTime.civil(@t.year, @t.mon, @t.day, 0, 0, 10)
    DateTime.strptime("10 09", '%H %S').should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 0, 9)
  end
  
  it "should be able to parse the number of seconds since the unix epoch" do
    DateTime.strptime("1226527410", '%s').should == DateTime.civil(2008, 11, 12, 22, 3, 30, 0)
    DateTime.strptime("1226527411", '%s').should == DateTime.civil(2008, 11, 12, 22, 3, 31, 0)
    DateTime.strptime("32511888001", "%s").should == DateTime.civil(3000, 4, 6, 0, 0, 1)
  end
  
  it "should be able to parse the time zone offset as a string of hours and minutes" do
    DateTime.strptime("2000 +0000", '%Y %z').offset.should == 0
    DateTime.strptime("2000 Z", '%Y %z').offset.should == 0
    DateTime.strptime("2000 -1200", '%Y %z').offset.should == -0.5
    DateTime.strptime("2000 +1200", '%Y %z').offset.should == 0.5
    DateTime.strptime("2000 -01:00", '%Y %z').offset.should == -1/24.0
    DateTime.strptime("2000 +01:00", '%Y %z').offset.should == 1/24.0
  end
  
  ############################
  # Specs that combine stuff #
  ############################

  it "should be able to parse the common date" do
    DateTime.strptime("Thu Apr  6 10:11:12 2000", "%c").should == DateTime.civil(2000, 4, 6, 10, 11, 12)  
  end

  it "should be able to parse the hour and minute" do
    DateTime.strptime("10:11", "%R").should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 11, 0)
  end

  it "should be able to parse the hour, minute, second, and am/pm flag" do
    DateTime.strptime("10:11:12 AM", "%r").should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 11, 12)
    DateTime.strptime("01:11:12 PM", "%r").should == DateTime.civil(@t.year, @t.mon, @t.day, 13, 11, 12)
  end

  it "should be able to parse the hour, minute, and second" do
    DateTime.strptime("10:11:12", "%T").should == DateTime.civil(@t.year, @t.mon, @t.day, 10, 11, 12)
    DateTime.strptime("01:11:12", "%X").should == DateTime.civil(@t.year, @t.mon, @t.day, 1, 11, 12)
  end
  
  it "should be able to parse the common date and timezone" do
    DateTime.strptime("Thu Apr  6 10:11:12 +0000 2000", "%+").should == DateTime.civil(2000, 4, 6, 10, 11, 12)
    DateTime.strptime("Thu Apr  6 10:11:12 +1200 2000", "%+").should == DateTime.civil(2000, 4, 6, 10, 11, 12, 0.5) 
  end
end
