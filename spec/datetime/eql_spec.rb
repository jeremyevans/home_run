require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#eql?" do
  it "should be able determine equality between date objects" do
    DateTime.civil(2007, 10, 11).should eql(DateTime.civil(2007, 10, 11))
    DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5).should eql(DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5))
    DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5).should eql(DateTime.civil(2007, 10, 12, 10, 11, 12, 0.5) - 1)
    DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5).should_not eql(DateTime.civil(2007, 10, 11, 10, 11, 12, 0.4))
  end

  it "should be able determine equality between a date and a datetime objects" do
    DateTime.civil(2007, 10, 11).should eql(Date.civil(2007, 10, 11))
    DateTime.civil(2007, 10, 11).should eql(Date.civil(2007, 10, 12) - 1)
    DateTime.civil(2007, 10, 11).should_not eql(Date.civil(2007, 10, 12))
    DateTime.civil(2007, 10, 11, 1).should_not eql(Date.civil(2007, 10, 11))
  end
end
