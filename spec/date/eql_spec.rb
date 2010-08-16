require File.expand_path('../../spec_helper', __FILE__)

describe "Date#eql?" do
  it "should be able determine equality between date objects" do
    Date.civil(2007, 10, 11).should eql(Date.civil(2007, 10, 11))
    Date.civil(2007, 10, 11).should eql(Date.civil(2007, 10, 12) - 1)
    Date.civil(2007, 10, 11).should_not eql(Date.civil(2007, 10, 12))
  end

  it "should be able determine equality between a date and a datetime objects" do
    Date.civil(2007, 10, 11).should eql(DateTime.civil(2007, 10, 11))
    Date.civil(2007, 10, 11).should eql(DateTime.civil(2007, 10, 12) - 1)
    Date.civil(2007, 10, 11).should_not eql(DateTime.civil(2007, 10, 12))
    Date.civil(2007, 10, 11).should_not eql(DateTime.civil(2007, 10, 11, 1))
  end
end
