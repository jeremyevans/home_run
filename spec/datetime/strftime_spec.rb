require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#strftime" do

  it "should be able to print the date time" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime.should == "2000-04-06T10:11:12+00:00"
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime.should == DateTime.civil(2000, 4, 6, 10, 11, 12).to_s
  end

  it "should be able to print the hour in a 24 hour clock with leading zero" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%H').should == "10"
    DateTime.civil(2000, 4, 6, 13, 11, 12).strftime('%H').should == "13"
  end

  it "should be able to print the hour in a 12 hour clock with leading zero" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%I').should == "10"
    DateTime.civil(2000, 4, 6, 13, 11, 12).strftime('%I').should == "01"
  end
  
  it "should be able to print the hour in a 24 hour clock with leading space" do
    DateTime.civil(2000, 4, 6, 9, 11, 12).strftime('%k').should == " 9"
    DateTime.civil(2000, 4, 6, 13, 11, 12).strftime('%k').should == "13"
  end

  it "should be able to print the hour in a 12 hour clock with leading space" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%l').should == "10"
    DateTime.civil(2000, 4, 6, 13, 11, 12).strftime('%l').should == " 1"
  end
  
  it "should be able to print the number of milliseconds of the second" do
    DateTime.civil(2008, 11, 12, 14, 3, 30, -8/24.0).strftime('%L').should == "000"
    (DateTime.civil(2008, 11, 12, 14, 3, 31, 8/24.0) + (0.5/86400)).strftime('%L').should == "500"
  end
  
  it "should be able to print the minute with leading zero" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%M').should == "11"
    DateTime.civil(2000, 4, 6, 10, 14, 12).strftime('%M').should == "14"
  end
  
  it "should be able to print the number of nanoseconds of the second" do
    DateTime.civil(2008, 11, 12, 14, 3, 30, -8/24.0).strftime('%N').should == "000000000"
    (DateTime.civil(2008, 11, 12, 14, 3, 31, 8/24.0) + (0.5/86400)).strftime('%N').should == "500000000"
  end
  
  it "should be able to print the meridian indicator in lower case" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%P').should == "am"
    DateTime.civil(2000, 4, 6, 13, 11, 12).strftime('%P').should == "pm"
  end
  
  it "should be able to print the meridian indicator in upper case" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%p').should == "AM"
    DateTime.civil(2000, 4, 6, 13, 11, 12).strftime('%p').should == "PM"
  end
  
  it "should be able to print the number of milliseconds since the unix epoch" do
    DateTime.civil(2008, 11, 12, 14, 3, 30, -8/24.0).strftime('%Q').should == "1226527410000"
    (DateTime.civil(2008, 11, 12, 14, 3, 31, 8/24.0) + (0.5/86400)).strftime('%Q').should == "1226469811500"
  end
  
  it "should be able to print the second with leading zero" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%S').should == "12"
    DateTime.civil(2000, 4, 6, 10, 11, 13).strftime('%S').should == "13"
  end
  
  it "should be able to print the number of seconds since the unix epoch" do
    DateTime.civil(2008, 11, 12, 14, 3, 30, -8/24.0).strftime('%s').should == "1226527410"
    DateTime.civil(2008, 11, 12, 14, 3, 31, 8/24.0).strftime('%s').should == "1226469811"
  end
  
  it "should be able to print the time zone offset as a string of hours and minutes" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%z').should == "+0000"
    DateTime.civil(2000, 4, 6, 10, 11, 12, -0.5).strftime('%z').should == "-1200"
    DateTime.civil(2000, 4, 6, 10, 11, 12, 0.5).strftime('%z').should == "+1200"
    DateTime.civil(2000, 4, 6, 10, 11, 12, -1/24.0).strftime('%z').should == "-0100"
    DateTime.civil(2000, 4, 6, 10, 11, 12, 1/24.0).strftime('%z').should == "+0100"
    
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%Z').should == "+00:00"
    DateTime.civil(2000, 4, 6, 10, 11, 12, -0.5).strftime('%Z').should == "-12:00"
    DateTime.civil(2000, 4, 6, 10, 11, 12, 0.5).strftime('%Z').should == "+12:00"
    DateTime.civil(2000, 4, 6, 10, 11, 12, -1/24.0).strftime('%Z').should == "-01:00"
    DateTime.civil(2000, 4, 6, 10, 11, 12, 1/24.0).strftime('%Z').should == "+01:00"
  end
  
  ############################
  # Specs that combine stuff #
  ############################

  it "should be able to print the common date" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime("%c").should == "Thu Apr  6 10:11:12 2000"
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime("%c").should == DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%a %b %e %H:%M:%S %Y')  
  end

  it "should be able to print the hour and minute" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime("%R").should == "10:11"
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime("%R").should == DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%H:%M')
  end

  it "should be able to show the hour, minute, second, and am/pm flag" do
    DateTime.civil(2000, 4,  9, 10, 11, 12).strftime("%r").should == "10:11:12 AM"
    DateTime.civil(2000, 4,  9, 13, 11, 12).strftime("%r").should == " 1:11:12 PM"
    DateTime.civil(2000, 4,  9, 10, 11, 12).strftime("%r").should == DateTime.civil(2000, 4,  9, 10, 11, 12).strftime('%I:%M:%S %p')
  end

  it "should be able to show the hour, minute, and second" do
    DateTime.civil(2000, 4,  9, 10, 11, 12).strftime("%T").should == "10:11:12"
    DateTime.civil(2000, 4,  9, 13, 11, 12).strftime("%T").should == "13:11:12"
    DateTime.civil(2000, 4,  9, 10, 11, 12).strftime("%T").should == DateTime.civil(2000, 4,  9, 10, 11, 12).strftime('%H:%M:%S')
    DateTime.civil(2000, 4,  9, 10, 11, 12).strftime("%X").should == "10:11:12"
    DateTime.civil(2000, 4,  9, 13, 11, 12).strftime("%X").should == "13:11:12"
    DateTime.civil(2000, 4,  9, 10, 11, 12).strftime("%X").should == DateTime.civil(2000, 4,  9, 10, 11, 12).strftime('%H:%M:%S')
  end
  
  it "should be able to print the common date and timezone" do
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime("%+").should == "Thu Apr  6 10:11:12 +00:00 2000"
    DateTime.civil(2000, 4, 6, 10, 11, 12, 0.5).strftime("%+").should == "Thu Apr  6 10:11:12 +12:00 2000"
    DateTime.civil(2000, 4, 6, 10, 11, 12).strftime("%+").should == DateTime.civil(2000, 4, 6, 10, 11, 12).strftime('%a %b %e %H:%M:%S %Z %Y')  
  end
end
